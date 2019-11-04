#include "WorkSpace.h"

#include <QCoreApplication>

#include "ServerLogger.h"
#include <MessageFactory.h>
#include <SharedException.h>


WorkSpace::WorkSpace(QSharedPointer<Document> d, QObject* parent)
	: doc(d), messageHandler(this), nFails(0)
{
	Logger() << "Loading document" << doc->getURI().toString();

	// Load the document contents
	doc->load();

	Logger() << "(LOAD COMPLETED)";

	// Start the auto-save timer
	timer.callOnTimeout<WorkSpace*>(this, &WorkSpace::documentSave);
	timer.start(DOCUMENT_SAVE_TIMEOUT);

	// Instantiate the Workspace thread and start it
	workThread = QSharedPointer<QThread>(new QThread(parent));
	connect(workThread.get(), &QThread::finished, workThread.get(), &QThread::deleteLater);
	this->moveToThread(workThread.get());
	workThread->start();
}

WorkSpace::~WorkSpace()
{
	timer.stop();			// Stop timer which is periodically saving the doc
	workThread->quit();		// Quit the thread
	workThread->wait();		// Waiting for ending the thread

	Logger() << "Saving and unloading document " << doc->getURI().toString();

	doc->save();			// Saving changes to the document before closing the workspace
	doc->unload();			// Unload the document contents from memory until it gets re-opened

	Logger() << "(COMPLETED)" << endl;
}


/****************************** CLIENT METHODS ******************************/

/* TcpServer sends to the Workspace a new client to edit the document */
void WorkSpace::newClient(QSharedPointer<Client> client)
{
	/* Get the active socket from the Client object */
	QSslSocket* socket = client->getSocket();
	socket->setParent(this);

	connect(socket, &QSslSocket::readyRead, this, &WorkSpace::readMessage);
	connect(socket, &QSslSocket::disconnected, this, &WorkSpace::clientDisconnection);
	connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &WorkSpace::socketErr);

	MessageFactory::DocumentReady(*doc)->send(socket);		// Send the document to the client

	// Send to the new user all the Presence messages of other editors in the workspace
	for (auto i = editors.begin(); i != editors.end(); ++i)
	{
		User* editor = i.value()->getUser();
		MessageFactory::PresenceAdd(editor->getUserId(), editor->getNickname(),
			editor->getIcon())->send(socket);
	}

	dispatchMessage(MessageFactory::PresenceAdd(client->getUserId(),				// Send to other clients this new presence
		client->getUser()->getNickname(), client->getUser()->getIcon()), socket);

	editors.insert(socket, client);

	Logger() << "User " << client->getUsername() << " opened the document";
}

/* Read an incoming message on the workspace socket and process it */
void WorkSpace::readMessage()
{
	QSslSocket* socket = dynamic_cast<QSslSocket*>(sender());
	QDataStream streamIn(socket);	/* connect stream with socket */
	QByteArray dataBuffer;

	if (!socketBuffer.getDataSize()) {
		streamIn >> socketBuffer;
	}

	// Read all the available message data from the socket
	dataBuffer = socket->read((qint64)(socketBuffer.getDataSize() - socketBuffer.getReadDataSize()));

	socketBuffer.append(dataBuffer);

	if (socketBuffer.bufferReadyRead())
	{
		QDataStream dataStream(&(socketBuffer.buffer), QIODevice::ReadWrite);
		quint16 mType = socketBuffer.getType();

		try {
			MessageCapsule message = MessageFactory::Empty((MessageType)mType);
			message->read(dataStream);
			socketBuffer.clear();

			if (mType == AccountUpdate || (mType >= CharInsert && mType <= PresenceRemove) || mType == DocumentClose)
			{
				messageHandler.process(message, socket);
			}
			else Logger(Error) << "(MESSAGE ERROR) Received unexpected message of type: " << mType;
		}
		catch (MessageException& me) 
		{
			clientQuit(socket, true);					// Remove the client from the document
			Logger(Error) << me.what();
			socketBuffer.clear();
			return;
		}
	}
}

/* Send the specified message to all clients except the one from which it was received (sender) */
void WorkSpace::dispatchMessage(MessageCapsule message, QSslSocket* sender)
{
	for (auto target = editors.keyBegin(); target != editors.keyEnd(); ++target)
	{
		if (*target != sender) {
			message->send(*target);
		}
	}
}

/* Handle client disconnection */
void WorkSpace::clientDisconnection()
{
	/* Close the socket where the signal was sent */
	QSslSocket* socket = dynamic_cast<QSslSocket*>(sender());

	QSharedPointer<Client> c = editors[socket];
	editors.remove(socket);
	socket->close();
	socket->deleteLater();
	Logger() << "Connection from client " << c->getUsername() << " was terminated";

	// Make this user avaiable to be logged-in again	
	emit userDisconnected(c->getUsername());

	// Send to other clients that this client is disconnected
	dispatchMessage(MessageFactory::PresenceRemove(c->getUserId()), nullptr);

	// If there are no more clients using this workspace, emit noEditors signal
	if (editors.isEmpty())
		emit noEditors(doc->getURI());
}

/* Print socket errors */
void WorkSpace::socketErr(QAbstractSocket::SocketError socketError)
{
	if (socketError != QAbstractSocket::RemoteHostClosedError)
		Logger(Error) << "(SOCKET ERROR) " << socketError;
}


/****************************** DOCUMENT METHODS ******************************/

/* Save document, if something goes wrong throw exception after MAX_FAILS */
void WorkSpace::documentSave()
{
	try
	{
		// Save the document's contents to file
		Logger() << "Saving document " << doc->getURI().toString();
		doc->save();

		Logger() << "(SAVE COMPLETED)";
		nFails = 0;
	}
	catch (DocumentException& de)
	{
		Logger(Error) << de.what() << ", fails count = " << nFails;
		if (nFails >= DOCUMENT_MAX_FAILS) {
			// Send Failure message to all clients in the workspace
			for each (QSslSocket * client in editors.keys())
				clientQuit(client, true);
		}
		nFails++;
	}
}

void WorkSpace::documentInsertSymbol(Symbol symbol)
{
	doc->insert(symbol);
}

void WorkSpace::documentDeleteSymbol(QVector<qint32> position)
{
	doc->remove(position);
}

void WorkSpace::documentEditSymbol(QVector<qint32> position, QTextCharFormat format)
{
	doc->formatSymbol(position, format);
}

void WorkSpace::documentEditBlock(TextBlockID blockId, QTextBlockFormat format)
{
	doc->formatBlock(blockId, format);
}

void WorkSpace::documentEditList(TextBlockID blockId, TextListID listId, QTextListFormat format)
{
	doc->editBlockList(blockId, listId, format);
}


/****************************** ACCOUNT METHODS ******************************/

/* Forwards to the main TcpServer the user request for an account update */
void WorkSpace::handleAccountUpdate(QSslSocket* clientSocket, QString nickname, QImage icon, QString password)
{
	QSharedPointer<Client> client = editors[clientSocket];

	emit requestAccountUpdate(client, nickname, icon, password);
}

/* Receives the TcpServer response to the account update message and sends it to the clients */
void WorkSpace::answerAccountUpdate(QSharedPointer<Client> client, MessageCapsule msg)
{
	QSslSocket* clientSocket = client->getSocket();

	if (msg->getType() == AccountConfirmed) {
		User* user = client->getUser();

		// Notify all other clients of the changes in this user's account
		dispatchMessage(MessageFactory::PresenceUpdate(user->getUserId(),
			user->getNickname(), user->getIcon()), clientSocket);
	}

	if (clientSocket->isOpen())
		msg->send(clientSocket);
}

/* Client close the document, must be re-send to TcpServer */
void WorkSpace::clientQuit(QSslSocket* clientSocket, bool isForced)
{
	QSharedPointer<Client> client = editors[clientSocket];

	editors.remove(clientSocket);			// Remove the client from the WorkSpace

	// Notify everyone else that this client exited the workspace
	dispatchMessage(MessageFactory::PresenceRemove(client->getUserId()), nullptr);

	if (isForced)
	{
		Logger() << "User " << client->getUsername() << " was removed from the document";

		// Send the Failure notification to the client
		MessageFactory::Failure("Server encountered an error")->send(clientSocket);
	}
	else
	{
		Logger() << "User " << client->getUsername() << " closed the document";

		// Send the DocumentExit confirmation to the client
		MessageFactory::DocumentExit()->send(clientSocket);
	}

	// Disconnect all the socket's signals from Workspace slots
	disconnect(clientSocket, &QSslSocket::readyRead, this, &WorkSpace::readMessage);
	disconnect(clientSocket, &QSslSocket::disconnected, this, &WorkSpace::clientDisconnection);	
	disconnect(clientSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &WorkSpace::socketErr);

	// Move the socket object back to the main server thread
	QSslSocket* s = client->getSocket();
	s->setParent(nullptr);
	s->moveToThread(QCoreApplication::instance()->thread());

	emit returnClient(std::move(client));		// Move the client back to the TcpServer

	if (editors.size() == 0)
		emit noEditors(doc->getURI());		// Close the workspace if nobody is editing the document
}

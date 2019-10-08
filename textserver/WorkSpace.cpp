#include "WorkSpace.h"

#include <QCoreApplication>

#include <MessageFactory.h>
#include "ServerException.h"
#include <SharedException.h>


WorkSpace::WorkSpace(QSharedPointer<Document> d, QObject* parent)
	: doc(d), messageHandler(this), nFails(0)
{
	qDebug() << endl << ">> Loading document" << doc->getURI().toString();

	// Load the document contents
	doc->load();

	qDebug() << ">> (LOAD COMPLETED)";

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

	qDebug() << ">> Saving and unloading document" << doc->getURI().toString();

	doc->save();			// Saving changes to the document before closing the workspace
	doc->unload();			// Unload the document contents from memory until it gets re-opened

	qDebug() << ">> (COMPLETED)" << endl;
}


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

	qDebug() << ">> User" << client->getUsername() << "opened the document";
}


// Read an incoming message on the workspace socket and process it
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
			else
			{
				qDebug() << ">> (ERROR) Received unexpected message of type: " << mType;
				message = MessageFactory::Failure(QString("Unknown message type : ") + QString::number(mType));
				message->send(socket);
			}
		}
		catch (MessageTypeException& mte) {
			MessageCapsule message = MessageFactory::Failure(QString("Unknown message type : ") + QString::number(mType));
			message->send(socket);
			qDebug().noquote() << ">>" << mte.what();
			socketBuffer.clear();
		}
		catch (MessageException& mre) {
			qDebug().noquote() << ">>" << mre.what();
			socketBuffer.clear();
			return;
		}
	}
}


// Send the specified message to all clients except the one from which it was received
void WorkSpace::dispatchMessage(MessageCapsule message, QSslSocket* sender)
{
	for (auto target = editors.keyBegin(); target != editors.keyEnd(); ++target)
	{
		if (*target != sender) {
			message->send(*target);
		}
	}
}


void WorkSpace::clientDisconnection()
{
	/* Close the socket where the signal was sent */
	QSslSocket* socket = dynamic_cast<QSslSocket*>(sender());

	QSharedPointer<Client> c = editors.find(socket).value();
	editors.remove(socket);
	socket->close();
	socket->deleteLater();
	qDebug() << ">> Connection from client" << c->getUsername() << "was closed abruptly";
	
	// Make this user avaiable to be logged again	
	emit userDisconnected(c->getUsername());

	// Send to other clients that this client is disconnected
	dispatchMessage(MessageFactory::PresenceRemove(c->getUserId()), nullptr);

	// If there are no more clients using this workspace, emit noEditors signal
	if (editors.isEmpty())
		emit noEditors(doc->getURI());
}


void WorkSpace::socketErr(QAbstractSocket::SocketError socketError)
{
	qDebug() << ">> (ERROR) Socket error: " << socketError;
}


void WorkSpace::documentSave()
{
	try
	{
		// Save the document's contents to file
		qDebug() << ">> Saving document" << doc->getURI().toString();
		doc->save();

		qDebug() << ">> (SAVE COMPLETED)";
		nFails = 0;
	}
	catch (DocumentException& de) 
	{
		qDebug().noquote() << ">" << de.what() << ", fails count =" << nFails ;
		if (nFails >= DOCUMENT_MAX_FAILS) {
			// Move Workspace clients back to TcpServer
			for (QSharedPointer<Client> client : editors.values()) {
				clientQuit(client->getSocket());
			}
		}
		nFails++;
	}
}

void WorkSpace::documentInsertSymbol(Symbol& symbol)
{
	doc->insert(symbol);
}

void WorkSpace::documentDeleteSymbol(QVector<qint32> position)
{
	doc->removeAt(position);
}

void WorkSpace::documentEditSymbol(QVector<qint32> position, QTextCharFormat format)
{
	doc->formatSymbol(position, format);
}

void WorkSpace::documentEditBlock(TextBlockID blockId, QTextBlockFormat format)
{
	doc->formatBlock(blockId, format);
}


/* Forwards to the main TcpServer the user request for an account update */
void WorkSpace::handleAccountUpdate(QSslSocket* clientSocket, QString nickname, QImage icon, QString password)
{
	QSharedPointer<Client> client = editors.find(clientSocket).value();

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
	
	if(clientSocket->isOpen())
		msg->send(clientSocket);
}


void WorkSpace::clientQuit(QSslSocket* clientSocket)
{
	QSharedPointer<Client> client = editors.find(clientSocket).value();

	editors.remove(clientSocket);			// Remove the client from the WorkSpace

	qDebug() << ">> User" << client->getUsername() << "closed the document";

	// Notify everyone else that this client exited the workspace
	dispatchMessage(MessageFactory::PresenceRemove(client->getUserId()), nullptr);

	// Send the DocumentExit confirmation to the client
	MessageFactory::DocumentExit()->send(clientSocket);

	// Delete the client's socket in the current thread
	disconnect(clientSocket, &QSslSocket::readyRead, this, &WorkSpace::readMessage);
	disconnect(clientSocket, &QSslSocket::disconnected, this, &WorkSpace::clientDisconnection);		// to avoid removing the socket twice
	disconnect(clientSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &WorkSpace::socketErr);

	// Move the socket object back to the main server thread
	QSslSocket* s = client->getSocket();
	s->setParent(nullptr);
	s->moveToThread(QCoreApplication::instance()->thread());

	emit returnClient(std::move(client));		// Move the client back to the TcpServer

	if (editors.size() == 0)
		emit noEditors(doc->getURI());		// Close the workspace if nobody is editing the document
}

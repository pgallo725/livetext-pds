#include "WorkSpace.h"

#include <QCoreApplication>

#include <MessageFactory.h>
#include "ServerException.h"


WorkSpace::WorkSpace(QSharedPointer<Document> d, QMutex& m, QObject* parent)
	: doc(d), messageHandler(this), users_mutex(m)
{
	doc->load();	// Load the document contents

	timer.callOnTimeout<WorkSpace*>(this, &WorkSpace::documentSave);
	timer.start(DOCUMENT_SAVE_TIMEOUT);

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
	doc->save();			// Saving changes to the document before closing the workspace
	doc->unload();			// Unload the document contents from memory until it gets re-opened
}


void WorkSpace::newClient(QSharedPointer<Client> client)
{
	/* get the active socket from the Client object */
	QSslSocket* socket = client->getSocket();
	socket->setParent(this);

	connect(socket, &QSslSocket::readyRead, this, &WorkSpace::readMessage);
	connect(socket, &QSslSocket::disconnected, this, &WorkSpace::clientDisconnection);

	socket->readAll();

	MessageFactory::DocumentReady(*doc)->sendTo(socket);		// Send the document to the client

	dispatchMessage(MessageFactory::PresenceAdd(client->getUserId(),				// Send to other clients this new presence
		client->getUser()->getNickname(), client->getUser()->getIcon()), socket);

	// Send to the new user all the Presence messages of other editors in the workspace
	for (auto i = editors.begin(); i != editors.end(); ++i)
	{
		User* editor = i.value()->getUser();
		MessageFactory::PresenceAdd(editor->getUserId(), editor->getNickname(),
			editor->getIcon())->sendTo(socket);
	}

	editors.insert(socket, client);
}


void WorkSpace::readMessage()
{
	QSslSocket* socket = dynamic_cast<QSslSocket*>(sender());
	QDataStream streamIn(socket);	/* connect stream with socket */
	QByteArray dataBuffer;

	if (!socketBuffer.getDataSize()) {
		streamIn >> socketBuffer;
	}

	dataBuffer = socket->read((qint64)(socketBuffer.getDataSize() - socketBuffer.getReadDataSize()));	// Read all the available message data from the socket

	socketBuffer.append(dataBuffer);

	if (socketBuffer.bufferReadyRead()) {
		QDataStream dataStream(&(socketBuffer.buffer), QIODevice::ReadWrite);
		quint16 mType = socketBuffer.getType();
		MessageCapsule message = MessageFactory::Empty((MessageType)mType);
		message->readFrom(dataStream);

		socketBuffer.clear();

		if (mType == LoginRequest || mType == LoginUnlock || mType == AccountCreate || mType == AccountUpdate ||
			mType == Logout || mType == DocumentCreate || mType == DocumentOpen || mType == DocumentRemove)
		{
			messageHandler.process(message, socket);
		}
		else
		{
			message = MessageFactory::Failure("Unknown message type : " + mType);
			message->sendTo(socket);
		}
	}
}


// Send the specified message to all connected clients except the one from which the message was received
void WorkSpace::dispatchMessage(MessageCapsule message, QSslSocket* sender)
{
	for (auto target = editors.keyBegin(); target != editors.keyEnd(); ++target)
	{
		if (*target != sender)
		{
			message->sendTo(*target);
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
	qDebug() << " - client '" << c->getUsername() << "' disconnected";
	
	// make this user avaiable to be logged again	
	emit restoreUserAvaiable(c->getUsername());

	// Send to other clients that this client is disconnected
	dispatchMessage(MessageFactory::PresenceRemove(c->getUserId()), nullptr);

	// If there are no more clients using this workspace, emit noEditors signal
	if (editors.isEmpty())
		emit noEditors(doc->getURI());
}


void WorkSpace::documentSave()
{
	doc->save();
}

void WorkSpace::documentInsertSymbol(Symbol& symbol)
{
	doc->insert(symbol);
}

void WorkSpace::documentDeleteSymbol(QVector<qint32> position)
{
	doc->removeAt(position);
}


/* Update user's fields and return response message for the client */
MessageCapsule WorkSpace::updateAccount(QSslSocket* clientSocket, QString nickname, QImage icon, QString password)
{
	QSharedPointer<Client> client = editors.find(clientSocket).value();

	if (!client->isLogged())
		return MessageFactory::AccountError("You are not logged in");

	User* user = client->getUser();
	QMutexLocker locker(&users_mutex);		// Modification of a user object must be done in mutex with the server thread

	if (!nickname.isEmpty())
		user->setNickname(nickname);
	if (!icon.isNull())
		user->setIcon(icon);
	if (!password.isEmpty())
		user->setPassword(password);

	// Notify all other clients of the changes in this user's account
	dispatchMessage(MessageFactory::PresenceUpdate(user->getUserId(),
		user->getNickname(), user->getIcon()), clientSocket);

	return MessageFactory::AccountConfirmed(*user);
}


void WorkSpace::clientQuit(QSslSocket* clientSocket)
{
	QSharedPointer<Client> client = editors.find(clientSocket).value();

	editors.remove(clientSocket);			// Remove the client from the WorkSpace

	qDebug() << " - client '" << client->getUsername() << "' closed the document";

	// Notify everyone else that this client exited the workspace
	dispatchMessage(MessageFactory::PresenceRemove(client->getUserId()), nullptr);

	// Delete the client's socket in the current thread
	disconnect(clientSocket, &QSslSocket::readyRead, this, &WorkSpace::readMessage);
	disconnect(clientSocket, &QSslSocket::disconnected, this, &WorkSpace::clientDisconnection);		// to avoid removing the socket twice

	// Move the socket object back to the main server thread
	QSslSocket* s = client->getSocket();
	s->setParent(nullptr);
	s->moveToThread(QCoreApplication::instance()->thread());

	emit returnClient(std::move(client));		// Move the client back to the TcpServer

	if (editors.size() == 0)
		emit noEditors(doc->getURI());		// Close the workspace if nobody is editing the document
}

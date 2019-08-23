#include "WorkSpace.h"

#include "TcpServer.h"
#include <MessageFactory.h>
#include "ServerException.h"


WorkSpace::WorkSpace(QSharedPointer<Document> d, QSharedPointer<TcpServer> server) 
	: doc(d), server(server), messageHandler(this)
{
	doc->load();	// Load the document contents

	timer.callOnTimeout<WorkSpace*>(this, &WorkSpace::documentSave);
	timer.start(SAVE_TIMEOUT);

	QThread* t = new QThread();
	connect(t, &QThread::finished, t, &QThread::deleteLater);
	this->moveToThread(t);
	t->start();
	workThread = QSharedPointer<QThread>(t);
}


WorkSpace::~WorkSpace()
{
	timer.stop();
	doc->save();	// Saving changes to the document before closing the workspace
	workThread->quit();
	workThread->wait();
}


void WorkSpace::newSocket(qint64 handle)
{
	QTcpSocket* socket = new QTcpSocket;
	QSharedPointer<Client> c;

	if (!socket->setSocketDescriptor(handle)) {
		qDebug() << socket->error();
		return;
	}
	
	if (!(c = server->moveClient(handle))) {
		qDebug() << "Client not found";
		//TODO: how to handle? throw ClientNotFoundException in moveClient
		return;
	}

	editors.insert(socket, c);
	dispatchMessage(MessageFactory::PresenceAdd(c->getUserId(),		// Send to other clients this new presence
		c->getUser()->getNickname(), c->getUser()->getIcon()), socket);

	connect(socket, &QTcpSocket::readyRead, this, &WorkSpace::readMessage);
	connect(socket, &QTcpSocket::disconnected, this, &WorkSpace::clientDisconnection);
}


void WorkSpace::readMessage()
{
	QTcpSocket* socket = static_cast<QTcpSocket*>(sender());

	QDataStream streamIn(socket);	/* connect stream with socket */

	quint16 mType;
	streamIn >> mType;		 /* take the type of incoming message */

	MessageCapsule message = MessageFactory::Empty((MessageType)mType);
	message->readFrom(streamIn);

	if (mType == CursorMove || mType == CharInsert || mType == CharDelete ||
		mType == AccountUpdate || mType == LogoutRequest)
	{
		messageHandler.process(message, socket);
	}
	else
	{
		message = MessageFactory::Failure("Unknown message type " + mType);
		message->sendTo(socket);
	}
}


// Send the specified message to all connected clients except the one from which the message was received
void WorkSpace::dispatchMessage(MessageCapsule message, QTcpSocket* sender)
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
	QTcpSocket* socket = static_cast<QTcpSocket*>(sender());

	QSharedPointer<Client> c = editors.find(socket).value();
	editors.remove(socket);
	qDebug() << "client removed";

	// Send to other clients that this client is disconnected
	dispatchMessage(MessageFactory::PresenceRemove(c->getUserId()), socket);

	// If there are no more clients using this workspace, emit notWorking signal
	if (!editors.size())
		emit notWorking(doc->getURI());		// TODO: consider adding a timer before closing the workspace
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
MessageCapsule WorkSpace::updateAccount(QTcpSocket* clientSocket, User& updatedUser)
{
	QSharedPointer<Client> client = editors.find(clientSocket).value();

	if (!client->isLogged())
		return MessageFactory::AccountError("You are not logged in");

	User* oldUser = client->getUser();

	if (oldUser->getUserId() == updatedUser.getUserId() &&
		oldUser->getUsername() == updatedUser.getUsername())
	{
		oldUser->setNickname(updatedUser.getNickname());
		oldUser->setIcon(updatedUser.getIcon());
		oldUser->changePassword(updatedUser.getPassword());

		// Notify all other clients of the changes in this user's account
		dispatchMessage(MessageFactory::PresenceUpdate(oldUser->getUserId(),
			oldUser->getNickname(), oldUser->getIcon()), clientSocket);

		return MessageFactory::AccountConfirmed(oldUser->getUserId());
	}
	else return MessageFactory::AccountError("Cannot modify a different user's account");
}


MessageCapsule WorkSpace::logoutClient(QTcpSocket* clientSocket)
{
	if (!editors.remove(clientSocket))
		return MessageFactory::LogoutError("You cannot logout if you're not logged in");

	if (editors.size() == 0)
		emit notWorking(doc->getURI());		// TODO: should probably be the last thing to do

	// TODO: need to give client to main thread
	return MessageFactory::LogoutConfirmed();
}

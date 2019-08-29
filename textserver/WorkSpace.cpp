#include "WorkSpace.h"

#include "TcpServer.h"
#include <MessageFactory.h>
#include "ServerException.h"


WorkSpace::WorkSpace(QSharedPointer<Document> d, QObject* parent)
	: doc(d), messageHandler(this)
{
	doc->load();	// Load the document contents

	timer.callOnTimeout<WorkSpace*>(this, &WorkSpace::documentSave);
	timer.start(SAVE_TIMEOUT);

	// TODO IGOR: creare il thread già dentro lo shared pointer ? forse da problemi con la connect/movetothread, appena riesco a testare questa parte provo
	QThread* t = new QThread(parent);
	connect(t, &QThread::finished, t, &QThread::deleteLater);
	this->moveToThread(t);
	t->start();
	workThread = QSharedPointer<QThread>(t);
}


WorkSpace::~WorkSpace()
{
	timer.stop();
	doc->save();			// Saving changes to the document before closing the workspace
	doc->unload();			// Unload the document contents from memory until it gets re-opened
	workThread->quit();		// Quit the thread
	workThread->wait();		// Waiting for ending the thread
}


/*void WorkSpace::newSocket(qint64 handle)
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

	// TODO IGOR: controllare l'ordine delle operazioni, la connect va fatta prima ?
	// inoltre l'if nel ciclo sarebbe evitabile se lo si fa prima di editors.insert(socket), fattibile ?
	editors.insert(socket, c);
	dispatchMessage(MessageFactory::PresenceAdd(c->getUserId(),			// Send to other clients this new presence
		c->getUser()->getNickname(), c->getUser()->getIcon()), socket);

	connect(socket, &QTcpSocket::readyRead, this, &WorkSpace::readMessage);
	connect(socket, &QTcpSocket::disconnected, this, &WorkSpace::clientDisconnection);

	// Send to the new user all the Presence messages of other editors in the workspace
	for (auto i = editors.begin(); i != editors.end(); ++i)
	{
		QTcpSocket* otherSocket = i.key();

		if (otherSocket != socket)
		{
			User* editor = i.value()->getUser();
			MessageFactory::PresenceAdd(editor->getUserId(), editor->getNickname(), editor->getIcon())->sendTo(socket);
		}
	}
}*/

void WorkSpace::newClient(QSharedPointer<Client> client)
{
	QTcpSocket* socket = new QTcpSocket;

	if (!socket->setSocketDescriptor(client->getSocketDescriptor())) {
		qDebug() << socket->error();
		return;
	}

	connect(socket, &QTcpSocket::readyRead, this, &WorkSpace::readMessage);
	connect(socket, &QTcpSocket::disconnected, this, &WorkSpace::clientDisconnection);

	MessageFactory::DocumentReady(*doc)->sendTo(socket);		// Send the document to the client

	dispatchMessage(MessageFactory::PresenceAdd(client->getUserId(),				// Send to other clients this new presence
		client->getUser()->getNickname(), client->getUser()->getIcon()), socket);

	// Send to the new user all the Presence messages of other editors in the workspace
	for (auto i = editors.begin(); i != editors.end(); ++i)
	{
		QTcpSocket* otherSocket = i.key();
		User* editor = i.value()->getUser();
		MessageFactory::PresenceAdd(editor->getUserId(), editor->getNickname(), editor->getIcon())->sendTo(socket);
	}

	editors.insert(socket, client);
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
		mType == PresenceRemove || mType == AccountUpdate)
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
	socket->close();
	socket->deleteLater();
	qDebug() << " - client '" << c->getUsername() << "' disconnected";
	
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
MessageCapsule WorkSpace::updateAccount(QTcpSocket* clientSocket, QString nickname, QImage icon, QString password)
{
	QSharedPointer<Client> client = editors.find(clientSocket).value();

	if (!client->isLogged())
		return MessageFactory::AccountError("You are not logged in");

	User* user = client->getUser();

	user->setNickname(nickname);
	user->setIcon(icon);
	user->changePassword(password);

	// Notify all other clients of the changes in this user's account
	dispatchMessage(MessageFactory::PresenceUpdate(user->getUserId(), nickname, icon), clientSocket);

	return MessageFactory::AccountConfirmed(*user);
}


void WorkSpace::clientQuit(QTcpSocket* clientSocket)
{
	QSharedPointer<Client> client = editors.find(clientSocket).value();

	editors.remove(clientSocket);			// Remove the client from the WorkSpace

	qDebug() << " - client '" << client->getUsername() << "' closed the document";

	// Notify everyone else that this client exited the workspace
	dispatchMessage(MessageFactory::PresenceRemove(client->getUserId()), nullptr);

	// Delete the client's socket in the current thread
	disconnect(clientSocket, &QTcpSocket::readyRead, this, &WorkSpace::readMessage);
	disconnect(clientSocket, &QTcpSocket::disconnected, this, &WorkSpace::clientDisconnection);		// to avoid removing the socket twice
	//clientSocket->deleteLater();

	emit returnClient(std::move(client));		// Move the client back to the TcpServer

	if (editors.size() == 0)
		emit noEditors(doc->getURI());		// Close the workspace if nobody is editing the document
}

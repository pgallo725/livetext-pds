#include "WorkSpace.h"

#include <memory>
#include "ServerException.h"
#include "TcpServer.h"


WorkSpace::WorkSpace(QSharedPointer<Document> d, QSharedPointer<TcpServer> server) 
	: doc(d), server(server), messageHandler(MessageHandler::Workspace, this)
{
	doc->load();	// Load the document contents

	timer.callOnTimeout<WorkSpace*>(this, &WorkSpace::saveDocument);
	timer.start(SAVE_TIMEOUT);
}


WorkSpace::~WorkSpace()
{
	timer.stop();
	doc->save();	// Saving changes to the document before closing the workspace
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
		//TODO: come gestiamo? throw ClientNotFoundException in moveClient
		return;
	}

	editors.insert(socket, c);
	// TODO: need to send to others clients this new presence

	connect(socket, &QTcpSocket::readyRead, this, &WorkSpace::readMessage);
	connect(socket, &QTcpSocket::disconnected, this, &WorkSpace::clientDisconnection);
}


/* Update user's fields */
bool WorkSpace::updateAccount(User* oldUser, User& newUser)
{
	if (oldUser->getUserId() == newUser.getUserId() &&
		oldUser->getUsername() == newUser.getUsername())
	{
		oldUser->setNickname(newUser.getNickname());
		oldUser->setIcon(newUser.getIcon());
		oldUser->changePassword(newUser.getPassword());

		return true;
	}
	else return false;
}


void WorkSpace::readMessage()
{
	QTcpSocket* socket = static_cast<QTcpSocket*>(sender());

	QDataStream streamIn(socket);	/* connect stream with socket */

	quint16 typeOfMessage;
	std::unique_ptr<Message> msg;

	streamIn >> typeOfMessage;		/* take the type of incoming message */


	// TODO: complete switch with others types of message
	switch (typeOfMessage)
	{
			/* Account messages */

		case AccountUpdate:
			msg = std::make_unique<AccountMessage>((MessageType)typeOfMessage);
			msg->readFrom(streamIn);
			break;

			/* Text messages */

		case CharInsert:
		case CharDelete:
			msg = std::make_unique<TextEditMessage>((MessageType)typeOfMessage);
			msg->readFrom(streamIn);
			break;

			/* Presence messages */

		case MoveCursor:
		case UserNameChange:
		case UserIconChange:
		case AddUserPresence:
		case RemoveUserPresence:
			//msg = std::make_unique<PresenceMessage>(typeOfMessage);
			msg->readFrom(streamIn);
			break;

			/* Logout messages */

		case LogoutRequest:
			msg = std::make_unique<LogoutMessage>((MessageType)typeOfMessage);
			msg->readFrom(streamIn);
			break;

			/* Unknown message type */

		default:
			msg = std::make_unique<ErrorMessage>(MessageTypeError, "Unknown message type " + typeOfMessage);
			msg->sendTo(socket);
			return;
	}

	messageHandler.process(std::move(msg), socket);
}


void WorkSpace::dispatchMessage(std::unique_ptr<Message>&& msg, QTcpSocket* sender)
{
	for (auto target = editors.keyBegin(); target != editors.keyEnd(); ++target)
	{
		if (*target != sender)
		{
			msg->sendTo(*target);
		}
	}
}


void WorkSpace::clientDisconnection()
{
	/* Close the socket where the signal was sent */
	QTcpSocket* socket = static_cast<QTcpSocket*>(sender());

	editors.remove(socket);
	qDebug() << "client removed";

	// TODO: need to send to others client that this client is disconnected

	// If there are no more clients using this workspace, emit notWorking signal
	if (!editors.size())
		emit notWorking(doc->getURI());		// TODO: consider adding a timer before closing the workspace
}


void WorkSpace::saveDocument()
{
	doc->save();
}

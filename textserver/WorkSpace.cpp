#include "WorkSpace.h"

#include <memory>
#include "ServerException.h"
#include "TcpServer.h"


WorkSpace::WorkSpace(QSharedPointer<Document> d, QSharedPointer<TcpServer> server) 
	: doc(d), server(server)
{
	doc->load();	// Load the document contents

	time.callOnTimeout<WorkSpace*>(this, &WorkSpace::saveDocument);
	time.start(SAVE_TIMEOUT);
}


WorkSpace::~WorkSpace()
{
	time.stop();
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
	
	if (!(c = server->moveClient(handle, doc->getURI()))) {
		qDebug() << "Client not found";
		//TODO: come gestiamo? throw ClientNotFoundException in moveClient
		return;
	}

	editors.insert(socket, c);

	connect(socket, &QTcpSocket::readyRead, this, &WorkSpace::readMessage);
	connect(socket, &QTcpSocket::disconnected, this, &WorkSpace::clientDisconnection);
}


/* update user's fields */
bool WorkSpace::updateAccount(User* user, quint16 typeField, QVariant field)
{
	switch (typeField) {
	case ChangeNickname:
		user->setNickname(field.value<QString>());
		break;

	case RemoveNickname:
		user->deleteNickname();
		break;

	case ChangeIcon:
		user->setIcon(field.value<QImage>());
		break;

	case RemoveIcon:
		user->deleteIcon();
		break;

	case ChangePassword:
		user->changePassword(field.value<QString>());
		break;

	default:
		return false;
		break;
	}
	return true;
}


void WorkSpace::readMessage()
{
	QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
	QDataStream streamIn;
	quint16 typeOfMessage;
	std::unique_ptr<Message> msg;

	streamIn.setDevice(socket); /* connect stream with socket */

	streamIn >> typeOfMessage;	/* take the type of incoming message */

	// TODO: complete switch with others types of message
	try {
		switch (typeOfMessage)
		{
			/* textMessages */
		case CharInsert:
			msg = std::make_unique<TextEditMessage>(CharInsert, streamIn);
			break;

		case CharDelete:
			msg = std::make_unique<TextEditMessage>(CharDelete, streamIn);
			break;

		case MoveCursor:
			break;

		case UserNameChange:
			break;

		case UserIconChange:
			break;

		case AddUserPresence:
			break;

		case RemoveUserPresence:
			break;

			/* Account messages */
		case AccountUpDate:
			msg = std::make_unique<AccountMessage>(AccountUpDate, streamIn);
			break;

			/* Logout messages */
		case LogoutRequest:
			msg = std::make_unique<LogoutMessage>(LogoutRequest);
			break;

		default:
			throw MessageUnknownTypeException(typeOfMessage);
			break;
		}

		handleMessage(std::move(msg), socket);
	}
	catch (MessageUnknownTypeException& e) {
		/* send to the client WrongMessageType */
		QDataStream streamOut;
		streamOut.setDevice(socket);
		streamOut << (quint16)WrongMessageType << e.getErrType();
	}
	catch (MessageWrongTypeException& e) {
		/* send to the client WrongMessageType + message */
		QDataStream streamOut;
		streamOut.setDevice(socket);
		QString err = e.what();
		streamOut << (quint16)WrongMessageType << err;
	}
	catch (MessageException& e) {
		/* TODO: client not found in create new Doc */
	}
	catch (SocketNullException& e) {
		// TODO
	}
}


void WorkSpace::handleMessage(std::unique_ptr<Message>&& msg, QTcpSocket* socket)
{
	QDataStream streamOut;
	quint16 typeOfMessage = 0;
	QString msg_str;

	if (socket == nullptr) throw SocketNullException("handleMessage reach null_ptr");

	streamOut.setDevice(socket);	/* connect stream with socket */

	switch (msg->getType()) {

	case CharInsert:
	{
		TextEditMessage* insertMsg = dynamic_cast<TextEditMessage*>(msg.get());
		doc->insert(insertMsg->getSymbol());
		break;
	}

	case CharDelete:
	{
		TextEditMessage* deleteMsg = dynamic_cast<TextEditMessage*>(msg.get());
		doc->removeAt(deleteMsg->getPosition());
		break;
	}

	case MoveCursor:
		break;

	case UserNameChange:
		break;

	case UserIconChange:
		break;

	case AddUserPresence:
		break;

	case RemoveUserPresence:
		break;

		/* Account messages */
	case AccountUpDate:
	{
		AccountMessage* accntUpdate = dynamic_cast<AccountMessage*>(msg.get());

		if (!editors.contains(socket))
			throw ClientNotFoundException("::handleMessage - client not found");

		if (!editors.find(socket).value()->isLogged()) {
			typeOfMessage = AccountDenied;
			msg_str = "client not logged";
		}
		else if (!updateAccount(editors.find(socket).value()->getUser(), accntUpdate->getFieldType(), accntUpdate->getField()))
		{
			typeOfMessage = AccountDenied;
			msg_str = "cannot modify this user";
		}
		else {
			typeOfMessage = AccountConfirmed;
			msg_str = "update completed";
		}

		streamOut << typeOfMessage << msg_str;
		break;
	}

		/* Logout messages */
	case LogoutRequest:
	{
		if (!editors.remove(socket)) {
			typeOfMessage = LogoutDenied;
			msg_str = "Cannot logout if you are already loggedout";
		}
		else {
			typeOfMessage = LogoutConfirmed;
			msg_str = "Logout complete";
			// TODO: need to give client to main thread
		}
		streamOut << typeOfMessage << msg_str;

		if (!editors.size())
			emit notWorking(doc->getURI());
		break;
	}
		
	default:
		throw MessageUnknownTypeException(msg->getType());
		break;
	}
}


void WorkSpace::clientDisconnection()
{
	/* Close the socket where the signal was sent */
	QTcpSocket* socket = static_cast<QTcpSocket*>(sender());

	editors.remove(socket);
	qDebug() << "client removed";

	// If there are no more clients using this workspace, emit notWorking signal
	if (!editors.size())
		emit notWorking(doc->getURI());		// TODO: consider adding a timer before closing the workspace
}

void WorkSpace::saveDocument()
{
	doc->save();
}

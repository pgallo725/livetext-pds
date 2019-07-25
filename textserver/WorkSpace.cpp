#include "WorkSpace.h"

#include "ServerException.h"
#include "TcpServer.h"

WorkSpace::WorkSpace(QSharedPointer<Document> d, QSharedPointer<TcpServer> server, QObject* parent):
	doc(d), server(server), QObject(parent)
{
}


WorkSpace::~WorkSpace()
{
}


void WorkSpace::newSocket(qint64 handle)
{
	QTcpSocket* socket = new QTcpSocket;

	if (!socket->setSocketDescriptor(handle)) {
		qDebug() << socket->error();
		return;
	}
	
	editors.insert(socket, server->getClient(handle));
	emit deleteClient(handle);

	connect(socket, &QTcpSocket::readyRead, this, &WorkSpace::readMessage);
	connect(socket, &QTcpSocket::disconnected, this, &WorkSpace::clientDisconnection);
}


void WorkSpace::readMessage()
{
	QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
	QDataStream streamIn;
	quint16 typeOfMessage;
	QSharedPointer<Message> msg;

	streamIn.setDevice(socket); /* connect stream with socket */

	streamIn >> typeOfMessage;	/* take the type of incoming message */

	// TODO: complete switch with others types of message
	try {
		switch (typeOfMessage)
		{
			/* textMessages */
		case CharInsert:
			break;

		case CharDelete:
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

		default:
			throw MessageUnknownTypeException(typeOfMessage);
			break;
		}

		handleMessage(msg, socket);
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


void WorkSpace::handleMessage(QSharedPointer<Message> msg, QTcpSocket* socket)
{
	QDataStream streamOut;
	quint16 typeOfMessage = 0;
	QString msg_str;

	if (socket == nullptr) throw SocketNullException("handleMessage reach null_ptr");

	streamOut.setDevice(socket); /* connect stream with socket */

	switch (msg->getType()) {
	
	default:
		throw MessageUnknownTypeException(typeOfMessage);
		break;
	}
}

void WorkSpace::clientDisconnection()
{
	/* Close the socket where the signal was sent */
	QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
	
	if (!editors.remove(socket)) {
		// TODO: editor not removed
	}

	qDebug() << "client removed";

	if(!editors.size())
		emit notWorking();
}
#include "LogoutMessage.h"


/*************** LOGOUT MESSAGE ***************/

LogoutMessage::LogoutMessage()
	: Message(Logout)
{
}

void LogoutMessage::readFrom(QDataStream& stream)
{
	// NO MESSAGE CONTENT
}

void LogoutMessage::sendTo(QTcpSocket* socket) const
{
	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);

	stream << Logout << (qint32)0;

	socket->write(buffer);
	socket->flush();
}


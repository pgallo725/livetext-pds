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
	QDataStream streamOut(socket);

	streamOut << (quint16)Logout << (int)0;
}


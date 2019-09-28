#include "LogoutMessage.h"


/*************** LOGOUT MESSAGE ***************/

LogoutMessage::LogoutMessage()
	: Message(Logout)
{
}

void LogoutMessage::writeContent(QDataStream& stream) const
{
	// NOTHING TO WRITE
}

void LogoutMessage::readFrom(QDataStream& stream)
{
	// NOTHING TO READ
}

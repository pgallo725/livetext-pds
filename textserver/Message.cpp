#include "Message.h"

Message::Message(QString message)
{
	msg = message;
}

Message::~Message()
{
}

int Message::getType()
{
	return 0;
}

int Message::getIP()
{
	return 0;
}

int Message::getPort()
{
	return 0;
}

QString Message::getUserName()
{
	return msg;
}

QString Message::getNickname()
{
	return QString();
}


QString Message::getPasswd()
{
	return QString();
}

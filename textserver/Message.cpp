#include "Message.h"


Message::Message(MessageType type) : m_type(type)
{
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
	return QString();
}

QString Message::getNickname()
{
	return QString();
}


QString Message::getPasswd()
{
	return QString();
}

#include "Message.h"


Message::Message(MessageType type) : m_type(type)
{
}

Message::~Message()
{
}

int Message::getType()
{
	return m_type;
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

QPixmap Message::getIcon()
{
	return QPixmap();
}

QString Message::getURI()
{
	return QString();
}

QString Message::getDocName()
{
	return QString();
}

quint16 Message::getFiledType()
{
	return quint16();
}

QString Message::getField()
{
	return quint16();
}

QPixmap Message::getNewIcon()
{
	return QPixmap();
}

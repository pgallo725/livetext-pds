#include "LoginMessage.h"

#include <QDataStream>

LoginMessage::LoginMessage(MessageType m, QDataStream& streamIn) : Message(m)
{
	switch (m)
	{

	case LoginRequest:
		streamIn >> m_username;
		break;
		
	case LoginUnlock:
		streamIn >> m_passwd;
	default:
		break;
	}
}

LoginMessage::~LoginMessage()
{
}

QString LoginMessage::getUserName()
{
	return m_username;
}

QString LoginMessage::getNickname()
{
	return m_nickname;
}


QString LoginMessage::getPasswd()
{
	return m_passwd;
}
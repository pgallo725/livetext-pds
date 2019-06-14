#include "LoginMessage.h"

#include <QDataStream>

LoginMessage::LoginMessage(MessageType m, QDataStream& streamIn) : Message(m)
{
	switch (m)
	{

	case LoginRequest:
		streamIn >> m_username;
		break;
	default:
		break;
	}
}

LoginMessage::~LoginMessage()
{
}

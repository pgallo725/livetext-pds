#include "LoginMessage.h"


LoginMessage::LoginMessage(MessageType m) 
	: Message(m)
{
}

LoginMessage::LoginMessage(MessageType loginType, QString payload)
	: Message(loginType), m_payload(payload)
{
}

LoginMessage::LoginMessage(MessageType loginGranted, User user)
	: Message(loginGranted), m_user(user)
{
}


void LoginMessage::readFrom(QDataStream& stream)
{
	switch (m_type)
	{
		case LoginRequest:
		case LoginChallenge:
		case LoginUnlock:
		case LoginError:
			stream >> m_payload;
			break;

		case LoginAccessGranted:
			stream >> m_user;
			break;

		default:
			// throw ?
			break;
	}
}

void LoginMessage::sendTo(QTcpSocket* socket)
{
	QDataStream streamOut(socket);

	streamOut << (quint16)m_type;

	switch (m_type)
	{
		case LoginRequest:
		case LoginChallenge:
		case LoginUnlock:
		case LoginError:
			streamOut << m_payload;
			break;

		case LoginAccessGranted:
			streamOut << m_user;
			break;

		default:
			// throw ?
			break;
	}
}


QString LoginMessage::getUsername()
{
	return m_payload;
}

QString LoginMessage::getNonce()
{
	return m_payload;
}

QString LoginMessage::getToken()
{
	return m_payload;
}

QString LoginMessage::getErrorMessage()
{
	return m_payload;
}

User LoginMessage::getLoggedUser()
{
	return m_user;
}

#include "LogoutMessage.h"


LogoutMessage::LogoutMessage(MessageType m) 
	: Message(m)
{
}

LogoutMessage::LogoutMessage(MessageType logoutError, QString reason)
	: Message(logoutError), m_reason(reason)
{
}


void LogoutMessage::readFrom(QDataStream& stream)
{
	switch (m_type)
	{
		case LogoutRequest:
		case LogoutConfirmed:
			break;

		case LogoutDenied:
			stream >> m_reason;
			break;

		default:
			// throw ?
			break;
	}
}

void LogoutMessage::sendTo(QTcpSocket* socket)
{
	QDataStream streamOut(socket);

	streamOut << (quint16)m_type;

	switch (m_type)
	{
		case LogoutRequest:
		case LogoutConfirmed:
			break;

		case LogoutDenied:
			streamOut << m_reason;
			break;

		default:
			// throw ?
			break;
	}
}


QString LogoutMessage::getErrorMessage()
{
	return m_reason;
}


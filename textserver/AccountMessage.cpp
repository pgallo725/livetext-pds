#include "AccountMessage.h"
#include "ServerException.h"


AccountMessage::AccountMessage(MessageType m)
	: Message(m), m_userId(-1)
{
}

AccountMessage::AccountMessage(MessageType accountCreateOrUpdate, User account)
	: Message(accountCreateOrUpdate), m_user(account), m_userId(-1)
{
}

AccountMessage::AccountMessage(MessageType accountConfirmed, int userId)
	: Message(accountConfirmed), m_userId(userId)
{
}

AccountMessage::AccountMessage(MessageType accountDenied, QString reason)
	: Message(accountDenied), m_reason(reason), m_userId(-1)
{
}


void AccountMessage::readFrom(QDataStream& stream)
{
	switch (m_type)
	{
		case AccountCreate:
			stream >> m_user;
			break;

		case AccountUpdate:
			stream >> m_user;
			break;

		case AccountConfirmed:
			stream >> m_userId;
			break;

		case AccountDenied:
			stream >> m_reason;
			break;

		default:
			throw MessageUnexpectedTypeException(m_type);	// ?
			break;
	}
}

void AccountMessage::sendTo(QTcpSocket* socket)
{
	QDataStream streamOut(socket);

	streamOut << (quint16)m_type;

	switch (m_type)
	{
		case AccountCreate:
			streamOut << m_user;
			break;

		case AccountUpdate:
			streamOut << m_user;
			break;

		case AccountConfirmed:
			streamOut << m_userId;
			break;

		case AccountDenied:
			streamOut << m_reason;
			break;

		default:
			throw MessageUnexpectedTypeException(m_type);	// ?
			break;
	}
}


User& AccountMessage::getUserObj()
{
	return m_user;
}

int AccountMessage::getUserId()
{
	if (m_type == AccountConfirmed)
		return m_userId;
	else return m_user.getUserId();
}

QString AccountMessage::getErrorMessage()
{
	return m_reason;
}


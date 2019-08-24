#include "AccountMessage.h"


/*************** ACCOUNT CREATE MESSAGE ***************/

AccountCreateMessage::AccountCreateMessage()
	: Message(AccountCreate)
{
}

AccountCreateMessage::AccountCreateMessage(User newUser)
	: Message(AccountCreate), m_user(newUser)
{
}

void AccountCreateMessage::readFrom(QDataStream& stream)
{
	stream >> m_user;
}

void AccountCreateMessage::sendTo(QTcpSocket* socket) const
{
	QDataStream streamOut(socket);

	streamOut << (quint16)AccountCreate << m_user;
}

User& AccountCreateMessage::getUserObj()
{
	return m_user;
}


/*************** ACCOUNT UPDATE MESSAGE ***************/

AccountUpdateMessage::AccountUpdateMessage()
	: Message(AccountUpdate)
{
}

AccountUpdateMessage::AccountUpdateMessage(User updatedUser)
	: Message(AccountUpdate), m_user(updatedUser)
{
}

void AccountUpdateMessage::readFrom(QDataStream& stream)
{
	stream >> m_user;
}

void AccountUpdateMessage::sendTo(QTcpSocket* socket) const
{
	QDataStream streamOut(socket);

	streamOut << (quint16)AccountUpdate << m_user;
}

User& AccountUpdateMessage::getUserObj()
{
	return m_user;
}


/*************** ACCOUNT CONFIRMED MESSAGE ***************/

AccountConfirmedMessage::AccountConfirmedMessage()
	: Message(AccountConfirmed), m_userId(-1)
{
}

AccountConfirmedMessage::AccountConfirmedMessage(qint32 userId)
	: Message(AccountConfirmed), m_userId(userId)
{
}

void AccountConfirmedMessage::readFrom(QDataStream& stream)
{
	stream >> m_userId;
}

void AccountConfirmedMessage::sendTo(QTcpSocket* socket) const
{
	QDataStream streamOut(socket);

	streamOut << (quint16)AccountConfirmed << m_userId;
}

qint32 AccountConfirmedMessage::getUserId() const
{
	return m_userId;
}


/*************** ACCOUNT DENIED MESSAGE ***************/

AccountErrorMessage::AccountErrorMessage()
	: Message(AccountError)
{
}

AccountErrorMessage::AccountErrorMessage(QString reason)
	: Message(AccountError), m_error(reason)
{
}

void AccountErrorMessage::readFrom(QDataStream& stream)
{
	stream >> m_error;
}

void AccountErrorMessage::sendTo(QTcpSocket* socket) const
{
	QDataStream streamOut(socket);

	streamOut << (quint16)AccountError << m_error;
}

QString AccountErrorMessage::getErrorMessage() const
{
	return m_error;
}

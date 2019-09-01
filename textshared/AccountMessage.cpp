#include "AccountMessage.h"

/*************** ACCOUNT CREATE MESSAGE ***************/

AccountCreateMessage::AccountCreateMessage()
	: Message(AccountCreate)
{
}

AccountCreateMessage::AccountCreateMessage(QString username, QString nickname, QImage icon, QString password)
	: Message(AccountCreate), m_username(username), m_nickname(nickname),
	m_icon(icon), m_password(password)
{
}

void AccountCreateMessage::readFrom(QDataStream& stream)
{
	stream >> m_username >> m_nickname >> m_password >> m_icon;
}

void AccountCreateMessage::sendTo(QTcpSocket* socket) const
{
	QDataStream streamOut(socket);
	QByteArray bufferData;
	QDataStream streamBuffer(&bufferData, QIODevice::WriteOnly);

	streamBuffer << m_username
		<< m_nickname
		<< m_password
		<< m_icon;

	streamOut << (quint16)AccountCreate << bufferData.size() << bufferData;

	/*
	streamOut << (quint16)AccountCreate << m_username 
		<< m_nickname << m_icon << m_password;
	*/
}

QString AccountCreateMessage::getUsername() const
{
	return m_username;
}

QString AccountCreateMessage::getNickname() const
{
	return m_nickname;
}

QImage AccountCreateMessage::getIcon() const
{
	return m_icon;
}

QString AccountCreateMessage::getPassword() const
{
	return m_password;
}


/*************** ACCOUNT UPDATE MESSAGE ***************/

AccountUpdateMessage::AccountUpdateMessage()
	: Message(AccountUpdate)
{
}

AccountUpdateMessage::AccountUpdateMessage(QString nickname, QImage icon, QByteArray password)
	: Message(AccountUpdate), m_nickname(nickname), m_icon(icon), m_password(password)
{
}

void AccountUpdateMessage::readFrom(QDataStream& stream)
{
	stream >> m_nickname >> m_password >> m_icon;
}

void AccountUpdateMessage::sendTo(QTcpSocket* socket) const
{
	QDataStream streamOut(socket);
	QByteArray bufferData;
	QDataStream streamBuffer(&bufferData, QIODevice::WriteOnly);

	streamBuffer << m_nickname
		<< m_password
		<< m_icon;

	streamOut << (quint16)AccountUpdate << bufferData.size() << bufferData;

	//streamOut << (quint16)AccountUpdate << m_nickname << m_icon << m_password;
}

QString AccountUpdateMessage::getNickname() const
{
	return m_nickname;
}

QImage AccountUpdateMessage::getIcon() const
{
	return m_icon;
}

QByteArray AccountUpdateMessage::getPassword() const
{
	return m_password;
}


/*************** ACCOUNT CONFIRMED MESSAGE ***************/

AccountConfirmedMessage::AccountConfirmedMessage()
	: Message(AccountConfirmed)
{
}

AccountConfirmedMessage::AccountConfirmedMessage(User user)
	: Message(AccountConfirmed), m_user(user)
{
}

void AccountConfirmedMessage::readFrom(QDataStream& stream)
{
	stream >> m_user;
}

void AccountConfirmedMessage::sendTo(QTcpSocket* socket) const
{
	QDataStream streamOut(socket);
	QByteArray bufferData;
	QDataStream streamBuffer(&bufferData, QIODevice::WriteOnly);

	streamBuffer << m_user;

	streamOut << (quint16)AccountConfirmed << bufferData.size() << bufferData;

	//streamOut << (quint16)AccountConfirmed << m_user;
}

User& AccountConfirmedMessage::getUserObj()
{
	return m_user;
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
	QByteArray bufferData;
	QDataStream streamBuffer(&bufferData, QIODevice::WriteOnly);

	streamBuffer << m_error;

	streamOut << (quint16)AccountError << bufferData.size() << bufferData;

	//streamOut << (quint16)AccountError << m_error;
}

QString AccountErrorMessage::getErrorMessage() const
{
	return m_error;
}

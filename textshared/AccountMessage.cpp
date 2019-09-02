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
	//QDataStream streamOut(socket);
	QByteArray dataBuffer;
	QDataStream streamBuffer(&dataBuffer, QIODevice::WriteOnly);

	streamBuffer << quint16(0) << qint32(0)
		<< m_username
		<< m_nickname
		<< m_password
		<< m_icon;

	streamBuffer.device()->seek(0);
	streamBuffer << (quint16)AccountCreate
				 << (qint32)dataBuffer.size() - sizeof(qint32);
	socket->write(dataBuffer);
	socket->flush();

	//streamOut << (quint16)AccountCreate << bufferData.size() << bufferData;

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

AccountUpdateMessage::AccountUpdateMessage(QString nickname, QImage icon, QString password)
	: Message(AccountUpdate), m_nickname(nickname), m_icon(icon), m_password(password)
{
}

void AccountUpdateMessage::readFrom(QDataStream& stream)
{
	stream >> m_nickname >> m_password >> m_icon;
}

void AccountUpdateMessage::sendTo(QTcpSocket* socket) const
{
	QByteArray dataBuffer;
	QDataStream streamBuffer(&dataBuffer, QIODevice::WriteOnly);

	streamBuffer << quint16(0) << qint32(0)
		<< m_nickname
		<< m_password
		<< m_icon;

	streamBuffer.device()->seek(0);
	streamBuffer << (quint16)AccountUpdate
		<< (qint32)dataBuffer.size() - sizeof(qint32);
	socket->write(dataBuffer);
	socket->flush();
	
	//streamOut << (quint16)AccountUpdate << bufferData.size() << bufferData;

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

QString AccountUpdateMessage::getPassword() const
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
	QByteArray dataBuffer;
	QDataStream streamBuffer(&dataBuffer, QIODevice::WriteOnly);

	streamBuffer << quint16(0) << qint32(0) << m_user;

	streamBuffer.device()->seek(0);
	streamBuffer << (quint16)AccountConfirmed
		<< (qint32)dataBuffer.size() - sizeof(qint32);
	socket->write(dataBuffer);
	socket->flush();

	//streamOut << (quint16)AccountConfirmed << bufferData.size() << bufferData;

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
	QByteArray dataBuffer;
	QDataStream streamBuffer(&dataBuffer, QIODevice::WriteOnly);

	streamBuffer << quint16(0) << qint32(0) << m_error;

	streamBuffer.device()->seek(0);
	streamBuffer << (quint16)AccountError
		<< (qint32)dataBuffer.size() - sizeof(qint32);
	socket->write(dataBuffer);
	socket->flush();

	//streamOut << (quint16)AccountError << bufferData.size() << bufferData;

	//streamOut << (quint16)AccountError << m_error;
}

QString AccountErrorMessage::getErrorMessage() const
{
	return m_error;
}

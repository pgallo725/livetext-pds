#include "LoginMessage.h"


/*************** LOGIN REQUEST MESSAGE ***************/

LoginRequestMessage::LoginRequestMessage()
	: Message(LoginRequest)
{
}

LoginRequestMessage::LoginRequestMessage(QString username)
	: Message(LoginRequest), m_username(username)
{
}

void LoginRequestMessage::readFrom(QDataStream& stream)
{
	stream >> m_username;
}

void LoginRequestMessage::sendTo(QSslSocket* socket) const
{
	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);

	stream << LoginRequest << quint32(0) << m_username;

	stream.device()->seek(sizeof(MessageType));
	stream << (quint32)(buffer.size() - sizeof(MessageType) - sizeof(quint32));
	socket->write(buffer);
	socket->flush();
}

QString LoginRequestMessage::getUsername() const
{
	return m_username;
}


/*************** LOGIN CHALLENGE MESSAGE ***************/

LoginChallengeMessage::LoginChallengeMessage()
	: Message(LoginChallenge)
{
}

LoginChallengeMessage::LoginChallengeMessage(QByteArray salt, QByteArray nonce)
	: Message(LoginChallenge), m_salt(salt), m_nonce(nonce)
{
}

void LoginChallengeMessage::readFrom(QDataStream& stream)
{
	stream >> m_salt >> m_nonce;
}

void LoginChallengeMessage::sendTo(QSslSocket* socket) const
{
	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);

	stream << LoginChallenge << quint32(0) << m_salt << m_nonce;

	stream.device()->seek(sizeof(MessageType));
	stream << (quint32)(buffer.size() - sizeof(MessageType) - sizeof(quint32));
	socket->write(buffer);
	socket->flush();
}

QByteArray LoginChallengeMessage::getSalt() const
{
	return m_salt;
}

QByteArray LoginChallengeMessage::getNonce() const
{
	return m_nonce;
}


/*************** LOGIN UNLOCK MESSAGE ***************/

LoginUnlockMessage::LoginUnlockMessage()
	: Message(LoginUnlock)
{
}

LoginUnlockMessage::LoginUnlockMessage(QByteArray token)
	: Message(LoginUnlock), m_token(token)
{
}

void LoginUnlockMessage::readFrom(QDataStream& stream)
{
	stream >> m_token;
}

void LoginUnlockMessage::sendTo(QSslSocket* socket) const
{
	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);

	stream << LoginUnlock << quint32(0) << m_token;

	stream.device()->seek(sizeof(MessageType));
	stream << (quint32)(buffer.size() - sizeof(MessageType) - sizeof(quint32));
	socket->write(buffer);
	socket->flush();
}

QByteArray LoginUnlockMessage::getToken() const
{
	return m_token;
}


/*************** LOGIN GRANTED MESSAGE ***************/

LoginGrantedMessage::LoginGrantedMessage()
	: Message(LoginGranted)
{
}

LoginGrantedMessage::LoginGrantedMessage(User loggedUser)
	: Message(LoginGranted), m_user(loggedUser)
{
}

void LoginGrantedMessage::readFrom(QDataStream& stream)
{
	stream >> m_user;
}

void LoginGrantedMessage::sendTo(QSslSocket* socket) const
{
	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);

	stream << LoginGranted << quint32(0) << m_user;

	stream.device()->seek(sizeof(MessageType));
	stream << (quint32)(buffer.size() - sizeof(MessageType) - sizeof(quint32));
	socket->write(buffer);
	socket->flush();
}

User& LoginGrantedMessage::getLoggedUser()
{
	return m_user;
}


/*************** LOGIN ERROR MESSAGE ***************/

LoginErrorMessage::LoginErrorMessage()
	: Message(LoginError)
{
}

LoginErrorMessage::LoginErrorMessage(QString reason)
	: Message(LoginError), m_error(reason)
{
}

void LoginErrorMessage::readFrom(QDataStream& stream)
{
	stream >> m_error;
}

void LoginErrorMessage::sendTo(QSslSocket* socket) const
{
	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);

	stream << LoginError << quint32(0) << m_error;

	stream.device()->seek(sizeof(MessageType));
	stream << (quint32)(buffer.size() - sizeof(MessageType) - sizeof(quint32));
	socket->write(buffer);
	socket->flush();
}

QString LoginErrorMessage::getErrorMessage() const
{
	return m_error;
}

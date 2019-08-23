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

void LoginRequestMessage::sendTo(QTcpSocket* socket) const
{
	QDataStream streamOut(socket);

	streamOut << (quint16)LoginRequest << m_username;
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

LoginChallengeMessage::LoginChallengeMessage(QString nonce)
	: Message(LoginChallenge), m_nonce(nonce)
{
}

void LoginChallengeMessage::readFrom(QDataStream& stream)
{
	stream >> m_nonce;
}

void LoginChallengeMessage::sendTo(QTcpSocket* socket) const
{
	QDataStream streamOut(socket);

	streamOut << (quint16)LoginChallenge << m_nonce;
}

QString LoginChallengeMessage::getNonce() const
{
	return m_nonce;
}


/*************** LOGIN UNLOCK MESSAGE ***************/

LoginUnlockMessage::LoginUnlockMessage()
	: Message(LoginUnlock)
{
}

LoginUnlockMessage::LoginUnlockMessage(QString token)
	: Message(LoginUnlock), m_token(token)
{
}

void LoginUnlockMessage::readFrom(QDataStream& stream)
{
	stream >> m_token;
}

void LoginUnlockMessage::sendTo(QTcpSocket* socket) const
{
	QDataStream streamOut(socket);

	streamOut << (quint16)LoginUnlock << m_token;
}

QString LoginUnlockMessage::getToken() const
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

void LoginGrantedMessage::sendTo(QTcpSocket* socket) const
{
	QDataStream streamOut(socket);

	streamOut << (quint16)LoginGranted << m_user;
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

void LoginErrorMessage::sendTo(QTcpSocket* socket) const
{
	QDataStream streamOut(socket);

	streamOut << (quint16)LoginError << m_error;
}

QString LoginErrorMessage::getErrorMessage() const
{
	return m_error;
}
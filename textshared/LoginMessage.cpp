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

void LoginRequestMessage::writeContent(QDataStream& stream) const
{
	stream << m_username;
}

void LoginRequestMessage::readFrom(QDataStream& stream)
{
	stream >> m_username;
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

void LoginChallengeMessage::writeContent(QDataStream& stream) const
{
	stream << m_salt << m_nonce;
}

void LoginChallengeMessage::readFrom(QDataStream& stream)
{
	stream >> m_salt >> m_nonce;
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

void LoginUnlockMessage::writeContent(QDataStream& stream) const
{
	stream << m_token;
}

void LoginUnlockMessage::readFrom(QDataStream& stream)
{
	stream >> m_token;
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

void LoginGrantedMessage::writeContent(QDataStream& stream) const
{
	stream << m_user;
}

void LoginGrantedMessage::readFrom(QDataStream& stream)
{
	stream >> m_user;
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

void LoginErrorMessage::writeContent(QDataStream& stream) const
{
	stream << m_error;
}

void LoginErrorMessage::readFrom(QDataStream& stream)
{
	stream >> m_error;
}

QString LoginErrorMessage::getErrorMessage() const
{
	return m_error;
}

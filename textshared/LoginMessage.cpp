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

	QBuffer bufferData;
	QDataStream streamBuffer(&bufferData);

	streamBuffer << m_username;

	streamOut << (quint16)LoginRequest << bufferData.size() << bufferData.data();

	//streamOut << (quint16)LoginRequest << m_username;
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

LoginChallengeMessage::LoginChallengeMessage(QString salt, QString nonce)
	: Message(LoginChallenge), m_salt(salt), m_nonce(nonce)
{
}

void LoginChallengeMessage::readFrom(QDataStream& stream)
{
	stream >> m_salt >> m_nonce;
}

void LoginChallengeMessage::sendTo(QTcpSocket* socket) const
{
	QDataStream streamOut(socket);
	QBuffer bufferData;
	QDataStream streamBuffer(&bufferData);

	streamBuffer << m_salt << m_nonce;

	streamOut << (quint16)LoginChallenge << bufferData.size() << bufferData.data();

	//streamOut << (quint16)LoginChallenge << m_nonce;
}

QString LoginChallengeMessage::getSalt() const
{
	return m_salt;
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
	QBuffer bufferData;
	QDataStream streamBuffer(&bufferData);

	streamBuffer << m_token;

	streamOut << (quint16)LoginUnlock << bufferData.size() << bufferData.data();

	//streamOut << (quint16)LoginUnlock << m_token;
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
	QBuffer bufferData;
	QDataStream streamBuffer(&bufferData);

	streamBuffer << m_user;

	streamOut << (quint16)LoginGranted << bufferData.size() << bufferData.data();

	//streamOut << (quint16)LoginGranted << m_user;
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
	QBuffer bufferData;
	QDataStream streamBuffer(&bufferData);

	streamBuffer << m_error;

	streamOut << (quint16)LoginError << bufferData.size() << bufferData.data();
	//streamOut << (quint16)LoginError << m_error;
}

QString LoginErrorMessage::getErrorMessage() const
{
	return m_error;
}

#pragma once
#ifndef _LOGIN_MESSAGE_
#define _LOGIN_MESSAGE_

#include "Message.h"
#include "User.h"


class LoginRequestMessage : public Message
{
	friend MessageFactory;

private:

	QString m_username;

protected:

	LoginRequestMessage();		// empty constructor

	// Costruct LoginRequest message with the username of the account that is trying to login
	LoginRequestMessage(QString username);

	void writeTo(QDataStream& stream) const override;
	void readFrom(QDataStream& stream) override;

public:

	~LoginRequestMessage() {};

	QString getUsername() const;
};



class LoginChallengeMessage : public Message
{
	friend MessageFactory;

private:

	QByteArray m_salt;
	QByteArray m_nonce;

protected:

	LoginChallengeMessage();		// empty constructor

	// Costruct LoginChallenge message with the cryptographic nonce
	LoginChallengeMessage(QByteArray salt, QByteArray nonce);

	void writeTo(QDataStream& stream) const override;
	void readFrom(QDataStream& stream) override;

public:

	~LoginChallengeMessage() {};

	QByteArray getSalt() const;
	QByteArray getNonce() const;
};



class LoginUnlockMessage : public Message
{
	friend MessageFactory;

private:

	QByteArray m_token;

protected:

	LoginUnlockMessage();		// empty constructor

	// Costruct LoginUnlock message with the solved cryptographic challenge
	LoginUnlockMessage(QByteArray token);

	void writeTo(QDataStream& stream) const override;
	void readFrom(QDataStream& stream) override;

public:

	~LoginUnlockMessage() {};

	QByteArray getToken() const;
};



class LoginGrantedMessage : public Message
{
	friend MessageFactory;

private:

	User m_user;

protected:

	LoginGrantedMessage();		// empty constructor

	// Costruct LoginGranted message with the account information for the user
	LoginGrantedMessage(User loggedUser);

	void writeTo(QDataStream& stream) const override;
	void readFrom(QDataStream& stream) override;

public:

	~LoginGrantedMessage() {};

	const User& getLoggedUser() const;
};



class LoginErrorMessage : public Message
{
	friend MessageFactory;

private:

	QString m_error;

protected:

	LoginErrorMessage();	 // empty constructor

	// Costruct LoginError message with a string as the error description
	LoginErrorMessage(QString reason);

	void writeTo(QDataStream& stream) const override;
	void readFrom(QDataStream& stream) override;

public:

	~LoginErrorMessage() {};

	QString getErrorMessage() const;
};
#endif

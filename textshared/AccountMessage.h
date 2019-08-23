#pragma once

#include "Message.h"
#include "User.h"


class AccountCreateMessage : public Message
{
	friend MessageFactory;

private:

	User m_user;

protected:

	AccountCreateMessage();		// empty constructor

	// Create an AccountCreate message, with the new account information
	AccountCreateMessage(User newUser);

public:

	~AccountCreateMessage() {};

	void readFrom(QDataStream& stream) override;
	void sendTo(QTcpSocket* socket) const override;

	User& getUserObj();
};



class AccountUpdateMessage : public Message
{
	friend MessageFactory;

private:

	User m_user;

protected:

	AccountUpdateMessage();		// empty constructor

	// Create an AccountUpdate message, with the new account information
	AccountUpdateMessage(User updatedUser);

public:

	~AccountUpdateMessage() {};

	void readFrom(QDataStream& stream) override;
	void sendTo(QTcpSocket* socket) const override;

	User& getUserObj();
};



class AccountConfirmedMessage : public Message
{
	friend MessageFactory;

private:

	qint32 m_userId;

protected:

	AccountConfirmedMessage();		// empty constructor

	// Construct the AccountConfirmed message, with the id chosen by the server for the User
	AccountConfirmedMessage(qint32 userId);

public:

	~AccountConfirmedMessage() {};

	void readFrom(QDataStream& stream) override;
	void sendTo(QTcpSocket* socket) const override;

	qint32 getUserId() const;
};



class AccountErrorMessage : public Message
{
	friend MessageFactory;

private:

	QString m_error;

protected:

	AccountErrorMessage();		// empty constructor

	// Create the AccountError message, with a string describing the error
	AccountErrorMessage(QString reason);

public:

	~AccountErrorMessage() {};

	void readFrom(QDataStream& stream) override;
	void sendTo(QTcpSocket* socket) const override;

	QString getErrorMessage() const;
};
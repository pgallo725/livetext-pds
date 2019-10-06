#pragma once

#include "Message.h"
#include "User.h"


class AccountCreateMessage : public Message
{
	friend MessageFactory;

private:

	QString m_username;
	QString m_nickname;
	QImage m_icon;
	QString m_password;

protected:

	AccountCreateMessage();		// empty constructor

	// Create an AccountCreate message, with the account information
	AccountCreateMessage(QString username, QString nickname, QImage icon, QString password);

	void writeTo(QDataStream& stream) const override;
	void readFrom(QDataStream& stream) override;

public:

	~AccountCreateMessage() {};

	QString getUsername() const;
	QString getNickname() const;
	QImage getIcon() const;
	QString getPassword() const;
};



class AccountUpdateMessage : public Message
{
	friend MessageFactory;

private:

	QString m_nickname;
	QImage m_icon;
	QString m_password;

protected:

	AccountUpdateMessage();		// empty constructor

	// Create an AccountUpdate message, with the new account information
	AccountUpdateMessage(QString nickname, QImage icon, QString password);

	void writeTo(QDataStream& stream) const override;
	void readFrom(QDataStream& stream) override;
public:

	~AccountUpdateMessage() {};

	QString getNickname() const;
	QImage getIcon() const;
	QString getPassword() const;
};



class AccountConfirmedMessage : public Message
{
	friend MessageFactory;

private:

	User m_user;

protected:

	AccountConfirmedMessage();		// empty constructor

	// Construct the AccountConfirmed message, with the User object just created or updated by the server
	AccountConfirmedMessage(User user);

	void writeTo(QDataStream& stream) const override;
	void readFrom(QDataStream& stream) override;
public:

	~AccountConfirmedMessage() {};

	User& getUserObj();
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

	void writeTo(QDataStream& stream) const override;
	void readFrom(QDataStream& stream) override;
public:

	~AccountErrorMessage() {};

	QString getErrorMessage() const;
};
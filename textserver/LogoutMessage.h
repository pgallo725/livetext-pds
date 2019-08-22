#pragma once

#include "Message.h"


class LogoutRequestMessage : public Message
{
	friend MessageFactory;

private:

protected:

	// Construct a LogoutRequest, no additional information needed
	LogoutRequestMessage();

public:

	~LogoutRequestMessage() {};

	void readFrom(QDataStream& stream) override;
	void sendTo(QTcpSocket* socket) const override;
};


class LogoutConfirmedMessage : public Message
{
	friend MessageFactory;

private:

protected:

	// Construct a LogoutConfirmed message, no additional information needed
	LogoutConfirmedMessage();

public:

	~LogoutConfirmedMessage() {};

	void readFrom(QDataStream& stream) override;
	void sendTo(QTcpSocket* socket) const override;
};


class LogoutErrorMessage : public Message
{
	friend MessageFactory;

private:

	QString m_reason;

protected:

	LogoutErrorMessage();		// empty constructor

	// Construct a LogoutError specifying the error reason as a string
	LogoutErrorMessage(QString reason);

public:

	~LogoutErrorMessage() {};

	void readFrom(QDataStream& stream) override;
	void sendTo(QTcpSocket* socket) const override;

	QString getErrorMessage() const;
};

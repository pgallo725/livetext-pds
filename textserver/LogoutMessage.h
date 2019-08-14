#pragma once

#include "Message.h"

class LogoutMessage : public Message
{

private:

	QString m_reason;

public:

	// Build an empty LogoutMessage, which can be either used as LogoutRequest
	// or LogoutConfirmed or instead filled with data read from the socket stream
	LogoutMessage(MessageType m);

	// Use this to construct LogoutError messages
	LogoutMessage(MessageType logoutError, QString reason);

	~LogoutMessage() {};


	void readFrom(QDataStream& stream) override;
	void sendTo(QTcpSocket* socket) override;


	/* getter */
	QString getErrorMessage();
};


#pragma once

#include "Message.h"


class LogoutMessage : public Message
{
	friend MessageFactory;

private:

protected:

	// Construct a LogoutRequest, no additional information needed
	LogoutMessage();

public:

	~LogoutMessage() {};

	void readFrom(QDataStream& stream) override;
	void sendTo(QSslSocket* socket) const override;
};

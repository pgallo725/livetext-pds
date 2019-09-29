#pragma once

#include "Message.h"


class LogoutMessage : public Message
{
	friend MessageFactory;

private:

protected:

	// Construct a LogoutRequest, no additional information needed
	LogoutMessage();

	void writeTo(QDataStream& stream) const override;

public:

	~LogoutMessage() {};

	void readFrom(QDataStream& stream) override;
};

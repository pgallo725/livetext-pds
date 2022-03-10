#pragma once
#ifndef _LOGOUT_MESSAGE_
#define _LOGOUT_MESSAGE_

#include "Message.h"


class LogoutMessage : public Message
{
	friend MessageFactory;

private:

protected:

	// Construct a LogoutRequest, no additional information needed
	LogoutMessage();

	void writeTo(QDataStream& stream) const override;
	void readFrom(QDataStream& stream) override;

public:

	~LogoutMessage() {};
	
};

#endif

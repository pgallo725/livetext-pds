#pragma once
#include "Message.h"
class LogoutMessage :
	public Message
{
private:
public:
	LogoutMessage(MessageType m);
	~LogoutMessage();
};


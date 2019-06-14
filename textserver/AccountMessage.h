#pragma once

#include "Message.h"
#include "User.h"

class AccountMessage :
	public Message
{
private:
	User user;

public:
	AccountMessage(MessageType m, QDataStream& streamIn);
	~AccountMessage();
};


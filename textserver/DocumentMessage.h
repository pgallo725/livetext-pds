#pragma once
#include <list>
#include "Message.h"
#include "User.h"

class DocumentMessage :
	public Message
{
private:
	QString docName;
	int docId;
	std::list<User> users;
public:
	DocumentMessage(MessageType m, QDataStream& streamIn);
	~DocumentMessage();
};


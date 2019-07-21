#pragma once

#include "Message.h"
#include "User.h"

class DocumentMessage :
	public Message
{
private:
	QString docName;
	QString URI;
	QString creatorName;
public:
	DocumentMessage(MessageType m, QDataStream& streamIn, QString username);
	~DocumentMessage();

	/* getter */
	QString getDocName();
	QString getURI();
	QString getUserName();
};



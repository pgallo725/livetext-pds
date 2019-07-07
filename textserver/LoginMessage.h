#pragma once
#include "Message.h"

class LoginMessage :
	public Message
{
private:
	
	QString m_nickname;
	QString m_passwd;
public:
	QString m_username;
	LoginMessage(MessageType m, QDataStream& streamIn);
	~LoginMessage();

	QString getUserName();
	QString getNickname();
	QString getPasswd();
};


#pragma once

#include <qtcpsocket.h>
#include "User.h"

class Client
{
	int clientId;
	QTcpSocket* socket;
	User* activeUser;

public:
	Client(int id, QTcpSocket* s, User* u);
	~Client();
};


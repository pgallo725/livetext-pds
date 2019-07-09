#pragma once

#include <qtcpsocket.h>
#include <QCryptographicHash>
#include "User.h"

class Client
{
	int clientId;
	QTcpSocket* socket;
	User* activeUser;
	bool logged;
	QString nonce;
public:
	Client(int id, QTcpSocket* s, User* u);
	~Client();

	/* getter */
	QTcpSocket* getSocket();
	User* getUser();
	QString getNonce();

	/* setter */
	void setLogged();
	void resetLogged();

	/* general methods */
	bool authentication(QString passwd);
};


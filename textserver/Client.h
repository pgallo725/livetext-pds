#pragma once

#include <qtcpsocket.h>
#include <QCryptographicHash>

#include "User.h"

class WorkSpace;	// inclusione anticipata

class Client
{
	int clientId;
	QTcpSocket* socket;
	User* activeUser;
	bool logged;
	QString nonce;
	std::shared_ptr<WorkSpace> workspace;
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
	bool setWorkspace(std::shared_ptr<WorkSpace> ws);
	void resetWorkspace();

	/* general methods */
	bool authentication(QString passwd);
};


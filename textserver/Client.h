#pragma once

#include <qtcpsocket.h>
#include <QCryptographicHash>

#include "User.h"

class WorkSpace;	// inclusione anticipata

class Client
{
	int clientId;
	qintptr socket;
	User* activeUser;
	bool logged;
	QString nonce;
	QSharedPointer<WorkSpace> workspace;
public:
	Client(int id, qintptr s, User* u);
	~Client();

	/* getter */
	qintptr getSocketDescriptor();
	User* getUser();
	QString getNonce();
	QString getUserName();

	/* setter */
	void setLogged();
	void resetLogged();
	bool setWorkspace(QSharedPointer<WorkSpace> ws);
	void resetWorkspace();

	/* general methods */
	bool authentication(QString passwd);
};


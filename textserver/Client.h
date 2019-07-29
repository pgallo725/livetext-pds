#pragma once

#include <QObject>
#include <qtcpsocket.h>
#include <QCryptographicHash>

#include "User.h"

class WorkSpace;	// inclusione anticipata

class Client //: public QObject
{
	//Q_OBJECT
private:
	int clientId;
	qintptr socket;
	User* activeUser;
	bool logged;
	QString nonce;
public:
	Client(int id, qintptr s, User* u);
	~Client();

	/* getter */
	int getUserId();
	qintptr getSocketDescriptor();
	User* getUser();
	QString getNonce();
	QString getUserName();

	/* setter */
	void setLogged();
	void resetLogged();

	/* general methods */
	bool authentication(QString passwd);
	bool isLogged();
};


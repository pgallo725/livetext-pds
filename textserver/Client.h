#pragma once

#include <qtcpsocket.h>
#include <QCryptographicHash>

#include "User.h"


class WorkSpace;	// forward declaration

class Client 
{

private:

	qintptr socket;
	User* activeUser;
	bool logged;
	QString nonce;

public:

	Client(qintptr s);
	~Client();

	/* getter */
	User* getUser();
	int getUserId();
	QString getUserName();
	qintptr getSocketDescriptor();
	QString getNonce();
	

	/* setter */
	void setUser(User* u);
	void setLogged();
	void resetLogged();

	/* general methods */
	bool authentication(QString passwd);
	void challenge(QTcpSocket* s);
	bool isLogged();
};


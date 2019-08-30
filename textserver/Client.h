#pragma once

#include "User.h"
#include <QTcpSocket>	

class WorkSpace;	// forward declaration

class Client 
{

private:

	qintptr socket;
	QTcpSocket* socketPtr;
	User* activeUser;
	bool logged;

	QString nonce;
	static const QString nonceCharacters;

public:

	Client(qintptr s);
	~Client();

	void login(User* user);
	void logout();

	bool authentication(QString token);
	QString challenge(User* user);
	bool isLogged();

	/* getters */
	User* getUser();
	int getUserId();
	QString getUsername();
	qintptr getSocketDescriptor();
	QTcpSocket* getSocketPtr();

	/* setters */
	void setSocketPtr(QTcpSocket* s);
};


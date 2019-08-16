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
};


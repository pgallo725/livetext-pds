#pragma once

#include "User.h"
#include <QTcpSocket>	


class Client 
{

private:

	QTcpSocket* socket;
	User* activeUser;
	bool logged;

	QString nonce;
	static const QString nonceCharacters;

public:

	Client(QTcpSocket* s);
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
	QTcpSocket* getSocket();

};


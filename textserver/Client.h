#pragma once

#include "User.h"
#include <QSslSocket>	

class Client 
{

private:

	QSslSocket* socket;
	User* activeUser;
	bool logged;

	QByteArray nonce;
	static const QString nonceCharacters;

public:

	Client(QSslSocket* s);
	~Client();

	void login(User* user);
	void logout();

	bool authentication(QByteArray token);
	QByteArray challenge(User* user);
	bool isLogged();

	/* getters */
	User* getUser();
	int getUserId();
	QString getUsername();
	qintptr getSocketDescriptor();
	QSslSocket* getSocket();

};


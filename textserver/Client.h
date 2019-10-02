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
	bool isLogged();

	bool authentication(QByteArray token);
	QByteArray challenge(User* user);

	/* getters */
	User* getUser();
	int getUserId();
	QString getUsername();
	QSslSocket* getSocket();
	qintptr getSocketDescriptor();

};


#pragma once
#ifndef __CLIENT__
#define __CLIENT__

#include "User.h"
#include <QSslSocket>	

#include "SocketBuffer.h"

class Client 
{

private:

	QSslSocket* socket;
	User* activeUser;
	bool logged;

	QByteArray nonce;
	static const QString nonceCharacters;

	SocketBuffer socketBuffer;

public:

	Client(QSslSocket* s);

	~Client();

	void login(User* user);
	void logout();
	bool isLogged();

	bool authenticate(QByteArray token);
	QByteArray challenge(User* user);

	/* getters */
	User* getUser() const;
	int getUserId() const;
	QString getUsername() const;
	QSslSocket* getSocket() const;
	qintptr getSocketDescriptor() const;
	SocketBuffer& getSocketBuffer();
};

#endif // __CLIENT__

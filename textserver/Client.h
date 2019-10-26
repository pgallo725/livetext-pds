#pragma once

#include <User.h>
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

	bool authenticate(QByteArray token);
	QByteArray challenge(User* user);

	/* getters */
	User* getUser() const;
	int getUserId() const;
	QString getUsername() const;
	QSslSocket* getSocket() const;
	qintptr getSocketDescriptor() const;

};


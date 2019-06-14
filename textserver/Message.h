#pragma once

#include <QObject>
#include <string>

#define BUFFLEN 256

// TODO: prototipo per far compilare

enum typeOfmessage
{
loginRequest,
loginChallenge,
LoginUnlock,
LoginAccessGranted,
LoginError,
AccountCreate,
AccountUpDate,
AccountConfirmed,
AccountDenied,
LogoutRequest,
LogoutConfimered
};

class Message 
{

private:
	QString msg;	/* oppure QString oppure QByteArray */

public:
	Message(QString message);
	~Message();

	/* getter */
	int getType();
	int getIP();
	int getPort();
	QString getUserName();
	QString getNickname();
	QString getPasswd();
};


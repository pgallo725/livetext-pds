#pragma once

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
	std::string msg;	/* oppure QString oppure QByteArray */

public:
	Message(std::string message);
	~Message();

	/* getter */
	int getType();
	int getIP();
	int getPort();
	std::string getUserName();
	std::string getName();
	std::string getSurname();
	std::string getPasswd();
};


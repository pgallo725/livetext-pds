#include "Client.h"
#include "Message.h"


Client::Client(qintptr s):
	socket(s), activeUser(nullptr), logged(false)
{
}

Client::~Client()
{
	// NOTHING TO DO, activeUser is owned by TcpServer and therefore it does NOT need to be destroyed
}

int Client::getUserId()
{
	return activeUser->getUserId();
}

qintptr Client::getSocketDescriptor()
{
	return socket;
}

User* Client::getUser()
{
	return activeUser;
}

QString Client::getUsername()
{
	if (!activeUser)
		return QString::null;
	else return activeUser->getUsername();
}

void Client::login(User* user)
{
	logged = true;
	activeUser = user;
}

void Client::logout()
{
	logged = false;
	activeUser = nullptr;
}

bool Client::authentication(QString token)
{
	QCryptographicHash hash(QCryptographicHash::Md5);

	hash.addData(activeUser->getPassword().toStdString().c_str(), activeUser->getPassword().length());
	hash.addData(this->nonce.toStdString().c_str(), this->nonce.length());

	return !QString::fromStdString(hash.result().toStdString()).compare(token);
}

QString Client::challenge(User* user)
{
	activeUser = user;
	nonce = "deadbeef";		// TODO: random sequence
	return nonce;
}

bool Client::isLogged()
{
	return logged;
}

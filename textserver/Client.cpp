#include "Client.h"


Client::Client(qintptr s, User& u):
	socket(s), activeUser(u), logged(false)
{
	// TODO: random sequence
	nonce = "deadbeef";
}

Client::~Client()
{
	// NOTHING TO DO, activeUser is owned by TcpServer and therefore it does NOT need to be destroyed
}

int Client::getUserId()
{
	return activeUser.getUserId();
}

qintptr Client::getSocketDescriptor()
{
	return socket;
}

User& Client::getUser()
{
	return activeUser;
}

QString Client::getNonce()
{
	return nonce;
}

QString Client::getUserName()
{
	return activeUser.getUsername();
}

void Client::setLogged()
{
	logged = true;
}

void Client::resetLogged()
{
	logged = false;
}

bool Client::authentication(QString passwd)
{
	QCryptographicHash hash(QCryptographicHash::Md5);

	hash.addData(activeUser.getPassword().toStdString().c_str(), activeUser.getPassword().length());
	hash.addData(this->nonce.toStdString().c_str(), this->nonce.length());

	return !QString::fromStdString(hash.result().toStdString()).compare(passwd);
}

bool Client::isLogged()
{
	return logged;
}

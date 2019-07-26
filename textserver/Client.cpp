#include "Client.h"

Client::Client(int id, qintptr s, User* u):
	clientId(id), socket(s), activeUser(u), logged(false), workspace(false)
{
	// TODO: random sequence
	nonce = "deadbeef";
}

Client::~Client()
{
}

qintptr Client::getSocketDescriptor()
{
	return socket;
}

User* Client::getUser()
{
	return activeUser;
}

QString Client::getNonce()
{
	return nonce;
}

QString Client::getUserName()
{
	return activeUser->getUsername();
}

void Client::setLogged()
{
	logged = true;
}

void Client::resetLogged()
{
	logged = false;
}

bool Client::setWorkspace(QSharedPointer<WorkSpace> ws)
{
	if (workspace == nullptr) return false;
	workspace = ws;
	return true;
}

void Client::resetWorkspace()
{
	workspace = nullptr;
}

bool Client::authentication(QString passwd)
{
	QCryptographicHash hash(QCryptographicHash::Md5);

	hash.addData(activeUser->getPassword().toStdString().c_str(), activeUser->getPassword().length());
	hash.addData(this->nonce.toStdString().c_str(), this->nonce.length());

	return !QString::fromStdString(hash.result().toStdString()).compare(passwd);
}
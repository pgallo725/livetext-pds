#include "Client.h"

Client::Client(int id, qintptr s,User* u):
	clientId(id), socket(s), activeUser(u), logged(false), workspace(nullptr)
{
	// TODO: random sequence
	nonce = "deadbeef";
}

Client::~Client()
{
}

int Client::getUserId()
{
	return clientId;
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
	WorkSpace* w = workspace.get();
	//if (workspace.isNull()) return false;
	//if (w == nullptr) return false;

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
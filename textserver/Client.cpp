#include "Client.h"

#include <QCryptographicHash>


// Set of characters that will be used to generate random sequences as nonce
const QString Client::nonceCharacters = QStringLiteral("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");


Client::Client(QTcpSocket* s):
	socket(s), activeUser(nullptr), logged(false)
{
}

Client::~Client()
{
	// NOTHING, activeUser is owned by TcpServer and therefore it does NOT need to be destroyed
}

int Client::getUserId()
{
	return activeUser->getUserId();
}

qintptr Client::getSocketDescriptor()
{
	return socket->socketDescriptor();
}

QTcpSocket* Client::getSocket()
{
	return socket;
}

User* Client::getUser()
{
	return activeUser;
}

QString Client::getUsername()
{
	if (activeUser == nullptr)
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

	hash.addData(activeUser->getPassword());
	hash.addData(this->nonce.toStdString().c_str(), this->nonce.length());

	return !QString::fromStdString(hash.result().toStdString()).compare(token);
}

QString Client::challenge(User* user)
{
	activeUser = user;		// store the user which is trying to login on this client

	for (int i = 0; i < 32; ++i)	// create a 32-character randomly generated nonce
	{
		int index = qrand() % nonceCharacters.length();
		QChar nextChar = nonceCharacters.at(index);
		nonce.append(nextChar);
	}

	return nonce;
}

bool Client::isLogged()
{
	return logged;
}

#include "Client.h"

#include <QCryptographicHash>


// Set of characters that will be used to generate random sequences as nonce
const QString Client::nonceCharacters = QStringLiteral("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");


Client::Client(QSslSocket* s):
	socket(s), activeUser(nullptr), logged(false)
{
}

Client::~Client()
{
	// NOTHING, activeUser is owned by TcpServer and therefore it does NOT need to be destroyed
}

QSslSocket* Client::getSocket()
{
	return socket;

}
qintptr Client::getSocketDescriptor()
{
	return socket->socketDescriptor();
}

User* Client::getUser()
{
	return activeUser;
}

int Client::getUserId()
{
	if (activeUser == nullptr)
		return -1;
	else return activeUser->getUserId();
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

bool Client::isLogged()
{
	return logged;
}

bool Client::authentication(QByteArray token)
{
	QCryptographicHash hash(QCryptographicHash::Md5);

	// Internally solve the challenge by MD5-hashing the nonce with
	// the stored user password and compare the results
	hash.addData(activeUser->getPasswordHash());
	hash.addData(this->nonce);

	return !hash.result().compare(token);
}

QByteArray Client::challenge(User* user)
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

#include "TcpServer.h"
#include <string>

bool TcpServer::login(std::string username, std::string passwd)
{
	std::map<std::string, User>::iterator it = users.find(username);

	if (it == users.end()) {
		return false;	/* no user with this name */
	}

	if (!users.at(username).authentication(passwd)) {
		return false;	/* password doesn't match */
	}

	return true;
}

std::optional<User> TcpServer::createNewAccount(std::string username, std::string name, std::string suername, std::string passwd)
{
	std::map<std::string, User>::iterator it = users.find(username);

	if (it != users.end()) {
		return std::optional<User>();
	}
	
	User nUser(name, suername, username, passwd, _userIdCounter++, 0);	/* create a new user		*/
	users.insert({ username, nUser });									/* insert new user in map	*/

	return nUser;
}

TcpServer::TcpServer(QObject* parent) : QObject(parent) , _userIdCounter(0){
	/* create a new object TCP server */
	textServer = new QTcpServer(this);
	
	/* connect newConnection from QTcpServer to newClientConnection() */
	connect(textServer, SIGNAL(newConnection()), this, SLOT(newClientConnection()));

	/* server listen on 0.0.0.0:9999 - return true on success */
	if (!textServer->listen(QHostAddress::Any, 9999))
	{
		qDebug() << "Server could not start";
	}
	else
	{
		/* take ip address and port */
		QString ip_address = textServer->serverAddress().toString();
		quint16 port = textServer->serverPort();
		qDebug() << "Server started at "<< ip_address <<":"<< port;
	}
}

TcpServer::~TcpServer()
{
	// TODO
}

/* handle a new connection from a client */
void TcpServer::newClientConnection()
{
	/* need to grab the socket - socket is created as a child of server */
	QTcpSocket* socket = textServer->nextPendingConnection();

	/* check if there's a new connection or it was a windows signal */
	if (socket == 0) {
		qDebug() << "ERROR: no pending connections!\n";
	}

	qDebug() << " - new connection from a client";

	/* get message login/register */
	if (socket->waitForReadyRead(TIMEOUT)) {
		QByteArray m = socket->readLine(BUFFLEN);
		qDebug() << m;

		Message msg(m.toStdString());

		if (msg.getType() == loginRequest) {
			/* login */
			if (login(msg.getUserName(), msg.getPasswd())) {
				/* access granted */
			}
			else {
				/* access denied */
			}
		}
		else if (msg.getType() == AccountCreate) {
			/* create a new account */
			if (auto newAccount = createNewAccount(msg.getUserName(), msg.getName(), msg.getSurname(), msg.getPasswd())) {
				/* new account created */
			}
			else {
				/* cannot create account with those credential */
			}
		}
	}

}

void TcpServer::clientDisconnection()
{
	/* take the object (socket) where the signal was send */
	QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
	socket->close();
}


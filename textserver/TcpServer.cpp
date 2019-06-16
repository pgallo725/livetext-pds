#include "TcpServer.h"

#include <optional>
#include <iostream>

#include <QString>
#include <QFile>
#include <QDataStream>
#include <QFileInfo>

#include "ServerException.h"


bool TcpServer::login(QString username, QString passwd)
{
	QMap<QString, User>::iterator it = users.find(username);

	if (it == users.end()) {
		return false;	/* no user with this name */
	}

	if (!(*it).authentication(passwd)) {
		return false;	/* password doesn't match */
	}

	return true;
}

std::optional<User> TcpServer::createNewAccount(QString username, QString nickname, QString passwd)
{
	QMap<QString, User>::iterator it = users.find(username);

	if (it != users.end()) {
		return std::optional<User>();
	}
	
	User nUser(username, nickname, passwd);			/* create a new user		*/
	users.insert(username, nUser);					/* insert new user in map	*/

	return nUser;
}

void TcpServer::saveUsers()
{
	// Create the new users database file and write the data to it
	QFile file(TMP_USERS_FILENAME);
	if (file.open(QIODevice::WriteOnly))
	{
		QDataStream usersDb(&file);

		std::cout << "\nSaving users database... ";

		// Write the the current users informations to file
		// using built-in Qt Map serialization
		usersDb << users;

		QFile oldFile(USERS_FILENAME);
		if (oldFile.exists())
		{
			if (oldFile.remove())
				file.rename(USERS_FILENAME);
			else
			{
				QFileInfo info(oldFile);
				throw FileOverwriteException(info.absoluteFilePath().toStdString());
			}
		}

		file.close();

		std::cout << "done" << std::endl;
	}
	else
	{
		QFileInfo info(file);
		throw FileWriteException(info.absolutePath().toStdString(), info.fileName().toStdString());
	}
}

TcpServer::TcpServer(QObject* parent) 
	: QObject(parent) , _userIdCounter(0)
{
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
		/* Get IP address and port */
		QString ip_address = textServer->serverAddress().toString();
		quint16 port = textServer->serverPort();
 		if (textServer->isListening()) {
			qDebug() << "Server started at " << ip_address << ":" << port;
		}
	}
}

TcpServer::~TcpServer()
{
	// TODO
}

void TcpServer::initialize()
{
	// Open the file and read the users database
	QFile file(USERS_FILENAME);
	if (file.open(QIODevice::ReadOnly))
	{
		std::cout << "\nLoading users database... ";

		QDataStream usersDbStream(&file);

		// Load the users database in the server's memory
		// using built-in Qt Map deserialization
		usersDbStream >> users;

		file.close();

		std::cout << "done" << std::endl;
	}
	else
	{
		QFileInfo info(file);
		throw FileLoadException(info.absoluteFilePath().toStdString());
	}
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

	connect(socket, SIGNAL(readyRead()), this, SLOT(readMessage()));
	connect(socket, SIGNAL(disconnected()), this, SLOT(clientDisconnection()));
}

void TcpServer::clientDisconnection()
{
	/* Close the socket where the signal was sent */
	QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
	socket->close();
	qDebug() << " - client disconnected";
}

void TcpServer::readMessage()
{
	QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
	QDataStream streamIn;
	quint16 typeOfMessage;
	Message* msg;

	streamIn.setDevice(socket); /* connect stream with socket */

	streamIn >> typeOfMessage;	/* take the type of incoming message */

	// TODO: complete switch with others types of message
	try {
		switch (typeOfMessage)
		{
			/* LoginMessages */
		case LoginRequest:
			msg = new LoginMessage(LoginRequest, streamIn);
			break;
		case LoginUnlock:
			msg = new LoginMessage(LoginUnlock, streamIn);
			break;
	
			/* AccountMessages */
		case AccountCreate:
			msg = new AccountMessage(AccountCreate, streamIn);
			break;
		case AccountUpDate:
			msg = new AccountMessage(AccountUpDate, streamIn);
			break;

			/* LogoutMessages */
		case LogoutRequest:
			msg = new LogoutMessage(LogoutRequest);
			break;



		default:
			throw MessageUnknownTypeException(typeOfMessage);
			break;
		}
	}
	catch (MessageUnknownTypeException& e) {
		(void)e;
		// TODO: handle exception
	}
	handleMessage(msg);
}

void TcpServer::handleMessage(Message* msg)
{
	
	if (msg->getType() == LoginRequest) {
		qDebug() << "si è loggato un coglione";//dynamic_cast<LoginMessage *>(msg)->n_username <<" si e' collegato: ";
	}
	// TODO: cast type message and handle it
}
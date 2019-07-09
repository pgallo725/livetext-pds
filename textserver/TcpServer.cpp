#include "TcpServer.h"

#include <optional>
#include <iostream>

#include <QString>
#include <QFile>
#include <QDataStream>
#include <QFileInfo>

#include "ServerException.h"


TcpServer::TcpServer(QObject* parent)
	: QObject(parent), _userIdCounter(0)
{
	/* create a new object TCP server */
	textServer = new QTcpServer(this);

	/* connect newConnection from QTcpServer to newClientConnection() */
	connect(textServer, &QTcpServer::newConnection, this, &TcpServer::newClientConnection);

	for (quint16 i = 1500; i < 10000; i += 101) {

		/* server listen on 0.0.0.0::i - return true on success */
		if (textServer->listen(QHostAddress::Any, i)) {
			break;
		}
	}

	if (!textServer->isListening())
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
	saveUsers();
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

		if (users.find("admin") == users.end()) {
			createNewAccount("admin", "sudo", "admin");
		}
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

	connect(socket, &QTcpSocket::readyRead, this, &TcpServer::readMessage);
	connect(socket, &QTcpSocket::disconnected, this, &TcpServer::clientDisconnection);
}


void TcpServer::clientDisconnection()
{
	/* Close the socket where the signal was sent */
	QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
	socket->close();
	qDebug() << " - client disconnected";
}


void TcpServer::sendLoginChallenge(QTcpSocket* socket, QString username)
{
	QDataStream streamOut;
	
	if (socket == nullptr) throw SocketNullException("handleMessage reach null_ptr");

	streamOut.setDevice(socket); /* connect stream with socket */

	Client client(_userIdCounter++, socket, &(*users.find(username)));
	clients.insert(socket, client);

	streamOut << (quint16)LoginChallenge << client.getNonce();
}


bool TcpServer::login(Client client, QString password)
{
	return client.authentication(password);
}


std::optional<User> TcpServer::createNewAccount(QString username, QString nickname, QString passwd, QTcpSocket* socket)
{
	QMap<QString, User>::iterator it = users.find(username);

	if (it != users.end()) {
		return std::optional<User>();
	}
	
	User nUser(username, nickname, passwd);			/* create a new user		*/
	users.insert(username, nUser);					/* insert new user in map	*/

	if (socket != nullptr) {
		if(clients.find(socket) != clients.end())	/* this socket is already use by another user */
			return std::optional<User>();
		Client client(_userIdCounter++, socket, &nUser);
		client.setLogged();
		clients.insert(socket, client);
	}

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


void TcpServer::readMessage()
{
	QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
	QDataStream streamIn;
	quint16 typeOfMessage;
	std::shared_ptr<Message> msg;

	streamIn.setDevice(socket); /* connect stream with socket */

	streamIn >> typeOfMessage;	/* take the type of incoming message */

	// TODO: complete switch with others types of message
	try {
		switch (typeOfMessage)
		{
			/* LoginMessages */
		case LoginRequest:
			msg = std::make_shared<LoginMessage>(LoginRequest, streamIn);
			break;
		case LoginUnlock:
			msg = std::make_shared<LoginMessage>(LoginUnlock, streamIn);
			break;
	
			/* AccountMessages */
		case AccountCreate:
			msg = std::make_shared<AccountMessage>(AccountCreate, streamIn);
			break;
		case AccountUpDate:
			msg = std::make_shared<AccountMessage>(AccountUpDate, streamIn);
			break;

			/* LogoutMessages */
		case LogoutRequest:
			msg =std::make_shared<LogoutMessage>(LogoutRequest);
			break;

			/* DocumentMessages */
		case NewDocument:
			break;

		case OpenDocument:
			break;

			/* textMessages */
		case CharInsert:
			break;

		case CharDelete:
			break;

		case MoveCursor:
			break;

		default:
			throw MessageUnknownTypeException(typeOfMessage);
			break;
		}

		handleMessage(msg, socket);
	}
	catch (MessageUnknownTypeException& e) {
		/* send to the client WrongMessageType */
		QDataStream streamOut;
		streamOut.setDevice(socket);
		streamOut << (quint16)WrongMessageType;
	}
	catch (MessageWrongTypeException& e) {
		/* send to the client WrongMessageType + message */
		QDataStream streamOut;
		streamOut.setDevice(socket);
		QString err = e.what();
		streamOut << (quint16)WrongMessageType << err;
	}
	catch (SocketNullException& e) {
		// TODO
	}
	
}


void TcpServer::handleMessage(std::shared_ptr<Message> msg, QTcpSocket* socket)
{
	QDataStream streamOut;
	quint16 typeOfMessage = 0;
	QString msg_str;

	if (socket == nullptr) throw SocketNullException("handleMessage reach null_ptr");

	streamOut.setDevice(socket); /* connect stream with socket */

	switch (msg->getType()) {
		/* Login */
	case LoginRequest:
		qDebug() << " si e' collegato: " << (msg)->getUserName();
		
		if (users.find(msg->getUserName()) == users.end()) 
			throw MessageWrongTypeException("User doesn't exist", WrongMessageType);

		sendLoginChallenge(socket, (msg)->getUserName());
		break;
	case LoginUnlock:

		if (login(*(clients.find(socket)), msg->getPasswd())) {
			/* login successful */
			clients.find(socket)->setLogged();
			typeOfMessage = LoginAccessGranted;
			msg_str = "Logged";
		}
		else {
			clients.remove(socket);
			typeOfMessage = LoginError;
			msg_str = "Wrong username/password";
		}
		streamOut << typeOfMessage << msg_str;
		break;

		/* Account */
	case AccountCreate:
		if (!createNewAccount(msg->getUserName(), msg->getNickname(), msg->getPasswd())) {
			/* something went wrong, maybe this user already exist*/
			typeOfMessage = AccountDenied;
			msg_str = "User already exist";
		}
		else {
			typeOfMessage = AccountConfirmed;
			msg_str = "Registration completed";
		}
		streamOut << typeOfMessage << msg_str;
		break;
	case AccountUpDate:
		break;

		/* LogoutMessages */
	case LogoutRequest:
		if (!clients.remove(socket)) {
			/* remove complete*/
			typeOfMessage = LogoutDenied;
			msg_str = "Cannot logout if you are already loggedout";
		}
		else {
			typeOfMessage = LogoutConfirmed;
			msg_str = "Logout complete";
		}
		streamOut << typeOfMessage << msg_str;
		break;

		/* DocumentMessages */
	case NewDocument:
		break;

	case OpenDocument:
		break;

		/* textMessages */
	case CharInsert:
		break;

	case CharDelete:
		break;

	case MoveCursor:
		break;

	default:
		throw MessageUnknownTypeException(typeOfMessage);
		break;
	
	}

	

	// TODO: cast type message and handle it
}





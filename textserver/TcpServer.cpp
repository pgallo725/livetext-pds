#include "TcpServer.h"

#include <optional>
#include <iostream>

#include <QFile>
#include <QDataStream>
#include <QFileInfo>

#include "ServerException.h"


TcpServer::TcpServer(QObject* parent)
	: QTcpServer(parent), _userIdCounter(0)
{
	/* connect newConnection from QTcpServer to newClientConnection() */
	connect(this, &QTcpServer::newConnection, this, &TcpServer::newClientConnection);

	for (quint16 i = 1500; i < 10000; i += 101) {

		/* server listen on 0.0.0.0::i - return true on success */
		if (this->listen(QHostAddress::Any, i)) {
			break;
		}
	}

	if (!this->isListening())
	{
		qDebug() << "Server could not start";
	}
	else
	{
		/* Get IP address and port */
		QString ip_address = this->serverAddress().toString();
		quint16 port = this->serverPort();
		if (this->isListening()) {
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
	QTcpSocket* socket = this->nextPendingConnection();

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
	clients.remove(socket);	/* remove this client from the list */
	socket->close();		/* close the socket */
	qDebug() << " - client disconnected";
}


void TcpServer::sendLoginChallenge(QTcpSocket* socket, QString username)
{
	QDataStream streamOut;
	
	if (socket == nullptr) throw SocketNullException("handleMessage reach null_ptr");
	
	streamOut.setDevice(socket); /* connect stream with socket */

	/* check if this socket is already used (user already logged) */
	if (clients.find(socket) != clients.end()) {
		QString msg_str = "Cannot log if you are logged";
		streamOut << (quint16)LoginError << msg_str;
		return;
	}

	Client* client = new Client(_userIdCounter++, socket, &(*users.find(username)));
	clients.insert(socket, QSharedPointer<Client>(client));

	streamOut << (quint16)LoginChallenge << client->getNonce();
}


bool TcpServer::login(QSharedPointer<Client> client, QString password)
{
	return client->authentication(password);
}


std::optional<User> TcpServer::createNewAccount(QString username, QString nickname, QString passwd, QTcpSocket* socket)
{
	QMap<QString, User>::iterator it = users.find(username);

	/* check if this user exist */
	if (it != users.end()) {
		return std::optional<User>();
	}
	
	User nUser(username, nickname, passwd);			/* create a new user		*/
	users.insert(username, nUser);					/* insert new user in map	*/

	/* check if socket is null (for static account) */
	if (socket != nullptr) {
		if(clients.find(socket) != clients.end())	/* this socket is already use by another user */
			return std::optional<User>();
		Client* client = new Client(_userIdCounter++, socket, &nUser);
		client->setLogged();
		clients.insert(socket, QSharedPointer<Client>(client));
	}

	return nUser;
}


bool TcpServer::createNewDocument(QString documentName, QString uri, QTcpSocket* author)
{
	if (documents.find(uri) != documents.end())
		return false;

	QSharedPointer<Client> c = clients.find(author).value();
	Document* doc = new Document(documentName, uri, c->getUserName());
	WorkSpace* w = new WorkSpace(QSharedPointer<Document>(doc));
	QThread *t = new QThread();

	documents.insert(uri, QSharedPointer<Document>(doc));
	workThreads.insert(uri, QSharedPointer<QThread>(t));

	/* change affinity of this workspace with a new thread */
	w->moveToThread(t);
	t->start();

	/* make the new thead connect the socket in the workspace */
	disconnect(author, &QTcpSocket::readyRead, this, &TcpServer::readMessage);	/* this thread will not recives more messages from client */
	connect(w, &WorkSpace::notWorking, this, &TcpServer::deleteWorkspace);		/* workspace will notify where theres no one using it */
	
	connect(this, &TcpServer::newSocket, w, &WorkSpace::newSocket);		
	emit newSocket(static_cast<qint64>(author->socketDescriptor()));	/* TODO: se emetto questo segnale non ho più il segnale dal socket quando il client disconnette */
	disconnect(this, &TcpServer::newSocket, w, &WorkSpace::newSocket);

	return true;
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
	QSharedPointer<Message> msg;

	streamIn.setDevice(socket); /* connect stream with socket */

	streamIn >> typeOfMessage;	/* take the type of incoming message */

	// TODO: complete switch with others types of message
	try {
		switch (typeOfMessage)
		{
			/* LoginMessages */
		case LoginRequest:
			msg = QSharedPointer<LoginMessage>(new LoginMessage(LoginRequest, streamIn));
			break;
		case LoginUnlock:
			msg = QSharedPointer<LoginMessage>(new LoginMessage(LoginUnlock, streamIn));
			break;
	
			/* AccountMessages */
		case AccountCreate:
			msg = QSharedPointer<AccountMessage>(new AccountMessage(AccountCreate, streamIn));
			break;
		case AccountUpDate:
			msg = QSharedPointer<AccountMessage>(new AccountMessage(AccountUpDate, streamIn));
			break;

			/* LogoutMessages */
		case LogoutRequest:
			msg = QSharedPointer<LogoutMessage>(new LogoutMessage(LogoutRequest));
			break;

			/* DocumentMessages */
		case NewDocument:
			//auto c = clients.find(socket);
			if (clients.find(socket) == clients.end()) throw MessageException("Client not found"); /* TODO: need a proper exception? */
			msg = QSharedPointer<DocumentMessage>(new DocumentMessage(NewDocument, streamIn, clients.find(socket).value()->getUserName()));
			break;

		case OpenDocument:
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
		streamOut << (quint16)WrongMessageType << e.getErrType();
	}
	catch (MessageWrongTypeException& e) {
		/* send to the client WrongMessageType + message */
		QDataStream streamOut;
		streamOut.setDevice(socket);
		QString err = e.what();
		streamOut << (quint16)WrongMessageType << err;
	}
	catch (MessageException& e) {
		/* TODO: client not found in create new Doc */
	}
	catch (SocketNullException& e) {
		// TODO
	}
	
}

void TcpServer::deleteWorkspace()
{
	// TODO: write on disk the file

}


void TcpServer::handleMessage(QSharedPointer<Message> msg, QTcpSocket* socket)
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
		
		/* check if this user exist or not */
		if (users.find(msg->getUserName()) == users.end()) 
			throw MessageWrongTypeException("User doesn't exist", WrongMessageType);

		sendLoginChallenge(socket, (msg)->getUserName());
		break;
	case LoginUnlock:
		if (login(clients.find(socket).value(), msg->getPasswd())) {
			/* login successful */
			clients.find(socket).value()->setLogged();
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
		if (!createNewAccount(msg->getUserName(), msg->getNickname(), msg->getPasswd(), socket)) {
			/* something went wrong, maybe this user already exist or this socket is already used */
			typeOfMessage = AccountDenied;
			msg_str = "User already exist or logged";
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
			typeOfMessage = LogoutDenied;
			msg_str = "Cannot logout if you are already loggedout";
		}
		else {
			/* remove complete*/
			typeOfMessage = LogoutConfirmed;
			msg_str = "Logout complete";
		}
		streamOut << typeOfMessage << msg_str;
		break;

		/* DocumentMessages */
	case NewDocument:

		if (!createNewDocument(msg->getDocName(), msg->getURI(), socket)) {
			typeOfMessage = DocumentError;
			msg_str = "Cannot create '"+msg->getDocName()+"', this document already exist";
		}
		else {
			typeOfMessage = DocumentOpened;
			msg_str = "Document created";
		}
		streamOut << typeOfMessage << msg_str;
		break;

	case OpenDocument:
		break;

	default:
		throw MessageUnknownTypeException(typeOfMessage);
		break;
	
	}
}





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
			createNewAccount("admin", "sudo", "admin", QPixmap());
		}
	}
	else
	{
		QFileInfo info(file);
		throw FileLoadException(info.absoluteFilePath().toStdString());
	}
}


QSharedPointer<Client> TcpServer::getClient(qintptr socketDescriptor)
{
	QSharedPointer<Client> c;

	for (QTcpSocket* it : clients.keys()) {
		if (clients.find(it).value()->getSocketDescriptor() == socketDescriptor) {
			c = std::move(clients.find(it).value());
			clients.remove(it);
			return c;
		}
	}

	return QSharedPointer<Client>();
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
	clients.find(socket).value().clear();	/* delete client object */
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

	Client* client = new Client(users.find(username).value().getUserId(), socket->socketDescriptor(), &(users.find(username).value()));
	clients.insert(socket, QSharedPointer<Client>(client));

	streamOut << (quint16)LoginChallenge << client->getNonce();
}


bool TcpServer::login(QSharedPointer<Client> client, QString password)
{
	return client->authentication(password);
}


bool TcpServer::logout(QTcpSocket* s)
{
	if (!clients.contains(s)) {
		return false;
	}

	clients.find(s).value().clear();
	clients.remove(s);
	return true;
}


bool TcpServer::createNewAccount(QString username, QString nickname, QString passwd, QPixmap icon, QTcpSocket* socket)
{
	QMap<QString, User>::iterator it = users.find(username);

	/* check if this user exist */
	if (it != users.end()) {
		return false;
	}
	
	User newUser(username, _userIdCounter++, nickname, passwd, icon);			/* create a new user		*/
	users.insert(username, newUser);					/* insert new user in map	*/

	/* check if socket is null (for static account) */
	if (socket != nullptr) {
		if (clients.find(socket) != clients.end())	/* this socket is already use by another user */
			return false;
		Client* client = new Client(newUser.getUserId(), socket->socketDescriptor(), &(users.find(username).value()));
		client->setLogged();
		clients.insert(socket, QSharedPointer<Client>(client));
	}

	return true;
}


bool TcpServer::updateAccount(User* user, quint16 typeField, QVariant field)
{
	switch (typeField) {
	case ChangeNickname:
		user->setNickname(field.value<QString>());
		break;

	case RemoveNickname:
		user->deleteNickname();
		break;

	case ChangeIcon:
		user->setIcon(field.value<QPixmap>());
		break;

	case RemoveIcon:
		user->deleteIcon();
		break;

	case ChangePassword:
		user->changePassword(field.value<QString>());
		break;

	default:
		return false;
		break;
	}
	return true;
}


bool TcpServer::createNewDocument(QString documentName, QString uri, QTcpSocket* author)
{
	if (documents.find(uri) != documents.end())
		return false;

	QSharedPointer<Client> c = clients.find(author).value();
	Document* doc = new Document(documentName, uri, c->getUserName());
	WorkSpace* w = new WorkSpace(QSharedPointer<Document>(doc), QSharedPointer<TcpServer>(this));
	QThread *t = new QThread();

	documents.insert(uri, QSharedPointer<Document>(doc));
	workThreads.insert(uri, QSharedPointer<QThread>(t));
	//c->setWorkspace(QSharedPointer<WorkSpace>(w));
	c->getUser()->addDocument(uri);

	/* change affinity of this workspace with a new thread */
	w->moveToThread(t);
	t->start();

	/* make the new thead connect the socket in the workspace */
	disconnect(author, &QTcpSocket::readyRead, this, &TcpServer::readMessage);	/* this thread will not recives more messages from client */
	disconnect(author, &QTcpSocket::disconnected, this, &TcpServer::clientDisconnection);

	connect(w, &WorkSpace::notWorking, this, &TcpServer::deleteWorkspace);		/* workspace will notify where theres no one using it */

	connect(this, &TcpServer::newSocket, w, &WorkSpace::newSocket);		
	emit newSocket(static_cast<qint64>(author->socketDescriptor()));	
	disconnect(this, &TcpServer::newSocket, w, &WorkSpace::newSocket);

	return true;
}


bool TcpServer::openDocument(QString uri, QTcpSocket* client)
{
	if (documents.find(uri) == documents.end())
		return false;

	Client* c = clients.find(client).value().get();
	WorkSpace* w;

	if (workspaces.find(uri) == workspaces.end()) {
		QThread* t = new QThread();
		w = new WorkSpace(documents.find(uri).value(), QSharedPointer<TcpServer>(this));
		workspaces.insert(uri, QSharedPointer<WorkSpace>(w));
		workThreads.insert(uri, QSharedPointer<QThread>(t));
		w->moveToThread(t);
		t->start();

		connect(w, &WorkSpace::notWorking, this, &TcpServer::deleteWorkspace);		
	}
	else {
		w = workspaces.find(uri).value().get();
	}

	c->setWorkspace(QSharedPointer<WorkSpace>(w));
	c->getUser()->addDocument(uri);

	disconnect(client, &QTcpSocket::readyRead, this, &TcpServer::readMessage);
	disconnect(client, &QTcpSocket::disconnected, this, &TcpServer::clientDisconnection);

	connect(this, &TcpServer::newSocket, w, &WorkSpace::newSocket);
	emit newSocket(static_cast<qint64>(client->socketDescriptor()));
	disconnect(this, &TcpServer::newSocket, w, &WorkSpace::newSocket);

	return true;
}


QStringList TcpServer::getUriFromUser(QString username)
{
	return users.find(username).value().getDocuments();
}


void TcpServer::deleteWorkspace(QString document)
{
	// TODO: write on disk the file
	WorkSpace* w = workspaces.find(document).value().get();
	QThread* t = workThreads.find(document).value().get();

	/* they throw an exception ??? why??? */
	//disconnect(w, &WorkSpace::notWorking, this, &TcpServer::deleteWorkspace);
	//disconnect(w, &WorkSpace::deleteClient, this, &TcpServer::deleteClient);
	workspaces.find(document).value().clear();
	workspaces.remove(document);	

	workThreads.find(document).value()->quit();
	workThreads.find(document).value()->wait();
	workThreads.find(document).value().clear();
	workThreads.remove(document);

	qDebug() << "workspace cancellato";
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
	std::unique_ptr<Message> msg;
	QString tmp;
	QSharedPointer<Client> c;

	streamIn.setDevice(socket); /* connect stream with socket */

	streamIn >> typeOfMessage;	/* take the type of incoming message */

	// TODO: complete switch with others types of message
	try {
		switch (typeOfMessage)
		{
			/* LoginMessages */
		case LoginRequest:
			msg = std::make_unique<LoginMessage>(LoginRequest, streamIn);
			break;
		case LoginUnlock:
			msg = std::make_unique<LoginMessage>(LoginUnlock, streamIn);
			break;
	
			/* AccountMessages */
		case AccountCreate:
			msg = std::make_unique<AccountMessage>(AccountCreate, streamIn);
			break;
		case AccountUpDate:
			msg = std::make_unique<AccountMessage>(AccountUpDate, streamIn);
			break;

			/* LogoutMessages */
		case LogoutRequest:
			msg = std::make_unique<LogoutMessage>(LogoutRequest);
			break;

			/* DocumentMessages */
		case NewDocument:
			c = clients.find(socket).value();
			tmp = clients.find(socket).value()->getUserName();

			if (clients.find(socket) == clients.end()) throw MessageException("Client not found"); /* TODO: need a proper exception? */
			msg = std::make_unique<DocumentMessage>(NewDocument, streamIn, clients.find(socket).value()->getUserName());
			break;

		case OpenDocument:
			if (clients.find(socket) == clients.end()) throw MessageException("Client not found"); /* TODO: need a proper exception? */
			msg = std::make_unique<DocumentMessage>(OpenDocument, streamIn, clients.find(socket).value()->getUserName());
			break;

		case UriRequest:
			if (clients.find(socket) == clients.end()) throw MessageException("Client not found"); /* TODO: need a proper exception? */
			msg = std::make_unique<DocumentMessage>(UriRequest, streamIn, clients.find(socket).value()->getUserName());
			break;

		default:
			throw MessageUnknownTypeException(typeOfMessage);
			break;
		}

		handleMessage(std::move(msg), socket);
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


void TcpServer::handleMessage(std::unique_ptr<Message>&& msg, QTcpSocket* socket)
{
	QDataStream streamOut;
	quint16 typeOfMessage = 0;
	QString msg_str;
	User u = User();

	if (socket == nullptr) throw SocketNullException("handleMessage reach null_ptr");

	streamOut.setDevice(socket); /* connect stream with socket */

	switch (msg->getType()) {

		/* Login */
	case LoginRequest:
	{
		LoginMessage* loginRequest = dynamic_cast<LoginMessage*>(msg.get());
		qDebug() << " si e' collegato: " << loginRequest->getUserName();

		/* check if this user exist or not */
		if (users.find(loginRequest->getUserName()) == users.end())
			throw MessageWrongTypeException("User doesn't exist", WrongMessageType);

		sendLoginChallenge(socket, loginRequest->getUserName());
		break;
	}
		
	case LoginUnlock:
	{
		LoginMessage* loginUnlock = dynamic_cast<LoginMessage*>(msg.get());
		if (login(clients.find(socket).value(), loginUnlock->getPasswd())) 
		{
			/* login successful */
			clients.find(socket).value()->setLogged();
			typeOfMessage = LoginAccessGranted;
			msg_str = "Logged";
			u = users.find(clients.find(socket).value()->getUserName()).value();
		}
		else {
			clients.remove(socket);
			typeOfMessage = LoginError;
			msg_str = "Wrong username/password";
		}
		streamOut << typeOfMessage << u << msg_str;
		break;
	}
		

		/* Account */
	case AccountCreate: {
		AccountMessage* accntCreate = dynamic_cast<AccountMessage*>(msg.get());
		if (!createNewAccount(accntCreate->getUserName(), accntCreate->getNickname(), 
			accntCreate->getPasswd(), accntCreate->getIcon(), socket))
		{
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
	}

	case AccountUpDate: 
	{
		AccountMessage* accntUpdate = dynamic_cast<AccountMessage*>(msg.get());
		if (!updateAccount(clients.find(socket).value()->getUser(), accntUpdate->getFieldType(), accntUpdate->getField()))
		{
			typeOfMessage = AccountDenied;
			msg_str = "cannot modify this user";
		}
		else {
			typeOfMessage = AccountConfirmed;
			msg_str = "update completed";
		}
		streamOut << typeOfMessage << msg_str;
		break;
	}
		

		/* LogoutMessages */
	case LogoutRequest:
	{
		if (!logout(socket)) {
			typeOfMessage = LogoutDenied;
			msg_str = "Cannot logout if you are already loggedout";
		}
		else {
			typeOfMessage = LogoutConfirmed;
			msg_str = "Logout complete";
		}
		streamOut << typeOfMessage << msg_str;
		break;
	}
		

		/* DocumentMessages */
	case NewDocument:
	{
		DocumentMessage* newDocument = dynamic_cast<DocumentMessage*>(msg.get());
		if (!createNewDocument(newDocument->getDocName(), newDocument->getURI(), socket)) 
		{
			typeOfMessage = DocumentError;
			msg_str = "Cannot create '" + newDocument->getDocName() + "', this document already exist";
		}
		else {
			typeOfMessage = DocumentOpened;
			msg_str = "Document created";
		}
		streamOut << typeOfMessage << msg_str;
		break;
	}

	case OpenDocument:
	{
		DocumentMessage* docMsg = dynamic_cast<DocumentMessage*>(msg.get());
		if (!openDocument(docMsg->getURI(), socket)) {
			typeOfMessage = DocumentError;
			msg_str = "Cannot open '" + docMsg->getDocName() + "', this document doesn't exist";
		}
		else {
			typeOfMessage = DocumentOpened;
			msg_str = "Document created";
		}
		streamOut << typeOfMessage << msg_str;
		break;
	}

	case UriRequest:
	{
		DocumentMessage* uriRequest = dynamic_cast<DocumentMessage*>(msg.get());
		typeOfMessage = UriResponse;
		streamOut << typeOfMessage << getUriFromUser(uriRequest->getUserName());
		break;
	}


	default:
		throw MessageUnknownTypeException(msg->getType());
		break;
	}
}


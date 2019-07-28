#include "TcpServer.h"

#include <optional>
#include <iostream>

#include <QFile>
#include <QDataStream>
#include <QFileInfo>

#include "ServerException.h"

/* costructor */
TcpServer::TcpServer(QObject* parent)
	: QTcpServer(parent), _userIdCounter(0)
{
	/* connect newConnection from QTcpServer to newClientConnection() */
	connect(this, &QTcpServer::newConnection, this, &TcpServer::newClientConnection);

	/* search a free port */
	for (quint16 i = 1500; i < 10000; i += 101) {

		/* server listen on 0.0.0.0::i - return true on success */
		if (this->listen(QHostAddress::Any, i)) {
			break;
		}
	}

	if (!this->isListening())
	{
		qDebug() << "Server could not start";
		throw ServerStartException("Server could not start");
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

/* destructor */
TcpServer::~TcpServer()
{
	// TODO
	saveUsers();
	// save docs
}

/* load clients and documents */
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

	//TODO: read document index file
}

/* save on disck users */
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

/* handle a new connection from a client */
void TcpServer::newClientConnection()
{
	/* need to grab the socket - socket is created as a child of server */
	QTcpSocket* socket = this->nextPendingConnection();

	/* check if there's a new connection or it was a windows signal */
	if (socket == 0) {
		qDebug() << "ERROR: no pending connections!\n";
		throw SocketNullException("::newClientConnection - no pending connections");
	}

	qDebug() << " - new connection from a client";

	/* connect slots to be able to read messages */
	connect(socket, &QTcpSocket::readyRead, this, &TcpServer::readMessage);
	connect(socket, &QTcpSocket::disconnected, this, &TcpServer::clientDisconnection);
}

/* handle client disconnection and releas resources */
void TcpServer::clientDisconnection()
{
	QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
	if (clients.contains(socket)) {
		clients.find(socket).value().clear();	/* delete client object */
		clients.remove(socket);					/* remove this client from the map */
	}
	socket->close();						/* close the socket */
	qDebug() << " - client disconnected";
}


/****************************** ACCOUNT METHODS ******************************/


/* create a new client and send nonce to be solved for authentication */
void TcpServer::sendLoginChallenge(QTcpSocket* socket, QString username)
{
	QDataStream streamOut;
	
	if (socket == nullptr) throw SocketNullException("::sendLoginChallenge - reach null_ptr");
	
	streamOut.setDevice(socket); /* connect stream with socket */

	/* check if this socket is already used (user already logged) */
	if (clients.contains(socket)) {
		QString msg_str = "Cannot log if you are logged";
		streamOut << (quint16)LoginError << msg_str;
		return;
	}

	Client* client = new Client(users.find(username).value().getUserId(), socket->socketDescriptor(), &(users.find(username).value()));
	clients.insert(socket, QSharedPointer<Client>(client));

	streamOut << (quint16)LoginChallenge << client->getNonce();
}

/* check authentication */
bool TcpServer::login(QSharedPointer<Client> client, QString password)
{
	return client->authentication(password);
}

/* release resources own by a client and delete it */
bool TcpServer::logout(QTcpSocket* s)
{
	if (!clients.contains(s)) {
		return false;
	}

	clients.find(s).value().clear();
	clients.remove(s);
	return true;
}

/* create a new User */
bool TcpServer::createNewAccount(QString username, QString nickname, QString passwd, QPixmap icon, QTcpSocket* socket)
{
	/* check if this username is already used */
	if (users.contains(username)) {
		return false;
	}
	
	User newUser(username, _userIdCounter++, nickname, passwd, icon);	/* create a new user		*/
	users.insert(username, newUser);									/* insert new user in map	*/

	/* check if socket is null										*
	*  static accont as Admin doesn't need a client during creation */
	if (socket != nullptr) {
		if (clients.contains(socket))	/* this socket is already use by another user */
			throw SocketDuplicatedException("::createNewAccount - socket already used");
			
		Client* client = new Client(newUser.getUserId(), socket->socketDescriptor(), &(users.find(username).value()));
		client->setLogged();			/* client is automatically logged */
		clients.insert(socket, QSharedPointer<Client>(client));
	}

	return true;
}

/* update user's fields */
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


/****************************** DOCUMENT METHODS ******************************/


/* create a new Document */
bool TcpServer::createNewDocument(QString documentName, QString uri, QTcpSocket* author)
{
	/* check if documents is already used */
	if (documents.contains(uri))
		return false;

	if (!clients.contains(author))
		throw ClientNotFoundException("::createNewDocument - client not found");

	QSharedPointer<Client> c = clients.find(author).value();
	Document* doc = new Document(documentName, uri, c->getUserName());
	WorkSpace* w = new WorkSpace(QSharedPointer<Document>(doc), QSharedPointer<TcpServer>(this));
	QThread *t = new QThread();

	documents.insert(uri, QSharedPointer<Document>(doc));
	workThreads.insert(uri, QSharedPointer<QThread>(t));
	c->getUser()->addDocument(uri);

	/* change affinity of this workspace with a new thread */
	w->moveToThread(t);
	t->start();

	/* this thread will not recives more messages from client */
	disconnect(author, &QTcpSocket::readyRead, this, &TcpServer::readMessage);	
	disconnect(author, &QTcpSocket::disconnected, this, &TcpServer::clientDisconnection);

	/* workspace will notify whene theres no one using it for delete */
	connect(w, &WorkSpace::notWorking, this, &TcpServer::deleteWorkspace);		

	/* make the new thead connect the socket in the workspace */
	connect(this, &TcpServer::newSocket, w, &WorkSpace::newSocket);		
	emit newSocket(static_cast<qint64>(author->socketDescriptor()));	
	disconnect(this, &TcpServer::newSocket, w, &WorkSpace::newSocket);

	return true;
}

/* open an existing Document */
bool TcpServer::openDocument(QString uri, QTcpSocket* client)
{
	/* check if this document doesn't exist */
	if (!documents.contains(uri))
		return false;

	if (!clients.contains(client))
		throw ClientNotFoundException("::openDocument - client not found");

	QSharedPointer<Client> c = clients.find(client).value();
	WorkSpace* w;

	/* check if this documents is already load in memory or not */
	if (!workspaces.contains(uri)) {	/* need to be load */
		QThread* t = new QThread();
		w = new WorkSpace(documents.find(uri).value(), QSharedPointer<TcpServer>(this));
		workspaces.insert(uri, QSharedPointer<WorkSpace>(w));
		workThreads.insert(uri, QSharedPointer<QThread>(t));
		w->moveToThread(t);
		t->start();

		/* workspace will notify whene theres no one using it for delete */
		connect(w, &WorkSpace::notWorking, this, &TcpServer::deleteWorkspace);		
	}
	else {								/* already load */
		w = workspaces.find(uri).value().get();
	}

	c->getUser()->addDocument(uri);

	/* this thread will not recives more messages from client */
	disconnect(client, &QTcpSocket::readyRead, this, &TcpServer::readMessage);
	disconnect(client, &QTcpSocket::disconnected, this, &TcpServer::clientDisconnection);

	/* make the new thead connect the socket in the workspace */
	connect(this, &TcpServer::newSocket, w, &WorkSpace::newSocket);
	emit newSocket(static_cast<qint64>(client->socketDescriptor()));
	disconnect(this, &TcpServer::newSocket, w, &WorkSpace::newSocket);

	return true;
}

/* get all the URI that a User owned */
QStringList TcpServer::getUriFromUser(QString username)
{
	if (!users.contains(username))
		throw UserNotFoundException("::openDocument - user not found");
	return users.find(username).value().getDocuments();
}

/* release all resource owned by workspace, delete it and its thread */
void TcpServer::deleteWorkspace(QString document)
{
	try {
		if (!workspaces.contains(document))
			throw WorkspaceNotFoundException("::deleteWorkspace - workspace '"+ document.toStdString() + "' not found");

		if (!workThreads.contains(document))
			throw ThreadNotFoundException("::deleteWorkspace - thread '" + document.toStdString() + "' not found");
	}
	catch (ObjectNotFound& e) {
		//TODO: need to be differentiate?
	}

	// TODO: write on disk the file
	WorkSpace* w = workspaces.find(document).value().get();
	QThread* t = workThreads.find(document).value().get();

	/* they throw an exception ??? why??? */
	//disconnect(w, &WorkSpace::notWorking, this, &TcpServer::deleteWorkspace);
	//disconnect(w, &WorkSpace::deleteClient, this, &TcpServer::deleteClient);

	/* delete workspace and remove it from the map */
	workspaces.find(document).value().clear();
	workspaces.remove(document);	

	/* stop the thread, wait for exit, delete thread and remove it from the map */
	workThreads.find(document).value()->quit();
	workThreads.find(document).value()->wait();
	workThreads.find(document).value().clear();
	workThreads.remove(document);

	qDebug() << "workspace cancellato";
}


/****************************** MESSAGES METHODS ******************************/


/* read the message from the socket and create the correct type message for the server */
void TcpServer::readMessage()
{
	QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
	QDataStream streamIn;
	quint16 typeOfMessage;
	std::unique_ptr<Message> msg;

	streamIn.setDevice(socket); /* connect stream with socket */

	streamIn >> typeOfMessage;	/* take the type of incoming message */

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
			if (!clients.contains(socket)) 
				throw ClientNotFoundException("::readMessage - client not found");
			msg = std::make_unique<DocumentMessage>(NewDocument, streamIn, clients.find(socket).value()->getUserName());
			break;

		case OpenDocument:
			if (!clients.contains(socket)) 
				throw ClientNotFoundException("::readMessage - client not found");
			msg = std::make_unique<DocumentMessage>(OpenDocument, streamIn, clients.find(socket).value()->getUserName());
			break;

		case UriRequest:
			if (!clients.contains(socket)) 
				throw ClientNotFoundException("::readMessage - client not found");
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
	catch (FieldWrongException& e) {
		//TODO: send by AccountMessage
	}
	catch (ClientNotFoundException& e) {
		//TODO
	}
	catch (UserNotFoundException& e) {
		//TODO: send by handleMessage
	}
	catch (SocketDuplicatedException& e) {
		//TODO: send by handleMessage -> createNewAccount
	}
	catch (MessageException& e) {
		/* TODO: client not found in create new Doc */
	}
	catch (SocketNullException& e) {
		// TODO
	}
	
}

/* handle the message create by readMessage */
void TcpServer::handleMessage(std::unique_ptr<Message>&& msg, QTcpSocket* socket)
{
	QDataStream streamOut;
	quint16 typeOfMessage = 0;
	QString msg_str;

	if (socket == nullptr) throw SocketNullException("::handleMessage - reach null_ptr");

	streamOut.setDevice(socket); /* connect stream with socket */

	switch (msg->getType()) {

		/* Login */
	case LoginRequest:
	{
		LoginMessage* loginRequest = dynamic_cast<LoginMessage*>(msg.get());
		qDebug() << " -- si e' collegato: " << loginRequest->getUserName();

		if (!users.contains(loginRequest->getUserName()))
			throw UserNotFoundException("::handleMessage - user not found");
		sendLoginChallenge(socket, loginRequest->getUserName());
		break;
	}
		
	case LoginUnlock:
	{
		if (!clients.contains(socket))
			throw ClientNotFoundException("::handleMessage - client not found");

		LoginMessage* loginUnlock = dynamic_cast<LoginMessage*>(msg.get());
		User u = User();
		QString username = clients.find(socket).value()->getUserName();

		if (!login(clients.find(socket).value(), loginUnlock->getPasswd())) 
		{
			/* login unsuccessful */
			logout(socket);
			typeOfMessage = LoginError;
			msg_str = "Wrong username/password";
		}
		else {
			/* login successful */
			clients.find(socket).value()->setLogged();
			typeOfMessage = LoginAccessGranted;
			msg_str = "Logged";
			if (!users.contains(username))
				throw UserNotFoundException("::handleMessage - user not found");
			u = users.find(username).value();
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
		
		if (!clients.contains(socket))
			throw ClientNotFoundException("::handleMessage - client not found");

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


/****************************** SERVER METHODS ******************************/


/* allow to a secondary thread to steal a QSharedPointer from the server */
QSharedPointer<Client> TcpServer::moveClient(qintptr socketDescriptor, QString workspace)
{
	QSharedPointer<Client> c;

	for (QTcpSocket* it : clients.keys()) {
		if (clients.find(it).value()->getSocketDescriptor() == socketDescriptor) {
			c = std::move(clients.find(it).value());
			clients.remove(it);
			//TODO: need to move thread affinity and make Client a QObject?
			return c;
		}
	}

	return QSharedPointer<Client>();
}

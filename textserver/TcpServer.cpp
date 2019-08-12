#include "TcpServer.h"

#include <optional>
#include <iostream>

#include <QFile>
#include <QDataStream>
#include <QFileInfo>
#include <QtNetwork>
#include <QHostAddress>
#include <QImage>

#include "ServerException.h"

/* Costructor */
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
		QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
		QString ip_address = this->serverAddress().toString();
		quint16 port = this->serverPort();
		if (this->isListening()) {
			qDebug() << "Server started\nreachable on:";
			for (const QHostAddress& address : ipAddressesList) {
				if (address.protocol() == QAbstractSocket::IPv4Protocol)
					qDebug() << "\t" << address.toString();
			}
			qDebug() << "Server listening at " << ip_address << ":" << port;
		}
	}
}

/* Destructor */
TcpServer::~TcpServer()
{
	time.stop();
	saveUsers();

	qDebug() << "Server down";
}

/* load clients and documents */
void TcpServer::initialize()
{
	// Open the file and read the users database
	QFile usersFile(USERS_FILENAME);
	if (usersFile.open(QIODevice::ReadOnly))
	{
		std::cout << "\nLoading users database... ";

		QDataStream usersDbStream(&usersFile);

		// Load the users database in the server's memory
		// using built-in Qt Map deserialization
		usersDbStream >> users;

		usersFile.close();

		std::cout << "done" << std::endl;

		if (users.find("admin") == users.end()) {
			createNewAccount("admin", "sudo", "admin", QImage());
		}
	}
	else
	{
		QFileInfo info(usersFile);
		throw FileLoadException(info.absoluteFilePath().toStdString());
	}


	// Read the documents' index file
	QFile docsFile(INDEX_FILENAME);
	if (docsFile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		std::cout << "\nLoading documents index file... ";

		QTextStream docIndexStream(&docsFile);

		// Load the document index in the server's memory
		while (!docIndexStream.atEnd())
		{
			QString docURI;

			docIndexStream >> docURI;

			if (docIndexStream.status() != QTextStream::Status::Ok)
			{
				// TODO: handle error, throw FileFormatException ?
			}

			// Create the actual Document object and store it in the server document map
			documents.insert(docURI, QSharedPointer<Document>(new Document(docURI)));
		}

		docsFile.close();

		std::cout << "done" << std::endl;
	}
	else
	{
		QFileInfo info(docsFile);
		throw FileLoadException(info.absoluteFilePath().toStdString());
	}

	// Setup timer for autosave
	time.callOnTimeout<TcpServer*>(this, &TcpServer::saveUsers);
	time.start(SAVE_TIMEOUT);

	std::cout << "\nServer up" << std::endl;
}

/* save on users on persistent storage */
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
		throw SocketNullException("::newClientConnection - no pending connections");		// could be ignored
	}

	qDebug() << " - new connection from a client";

	QSharedPointer<Client> client(new Client(socket->socketDescriptor()));
	clients.insert(socket, client);

	/* connect slots to be able to read messages */
	connect(socket, &QTcpSocket::readyRead, this, &TcpServer::readMessage);
	connect(socket, &QTcpSocket::disconnected, this, &TcpServer::clientDisconnection);
}

/* handle client disconnection and releas resources */
void TcpServer::clientDisconnection()
{
	QTcpSocket* socket = static_cast<QTcpSocket*>(sender());

	clients.remove(socket);					/* remove this client from the map */
	socket->close();						/* close the socket */

	qDebug() << " - client disconnected";
}


/****************************** ACCOUNT METHODS ******************************/


/* create a new client and send nonce to be solved for authentication */
void TcpServer::sendLoginChallenge(QTcpSocket* socket, QString username)
{
	QSharedPointer<Client> c = clients.find(socket).value();
	c->setUser(&(users.find(username).value()));
	c->challenge(socket);
}

/* check authentication */
bool TcpServer::login(QSharedPointer<Client> client, QString password)
{
	return client->authentication(password);
}

/* release resources own by a client and delete it */
bool TcpServer::logout(QTcpSocket* s)
{
	QSharedPointer<Client> c = clients.find(s).value();
	if (!c->isLogged()) {
		return false;
	}

	c->resetLogged();
	return true;
}

/* create a new User */
bool TcpServer::createNewAccount(QString username, QString nickname, QString passwd, QImage icon, QTcpSocket* socket)
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
		QSharedPointer<Client> client = clients.find(socket).value();
		client->setUser(&(users.find(username).value()));
		client->setLogged();				// client is automatically logged
	}

	return true;
}

/* Update user's fields */
bool TcpServer::updateAccount(User* user, quint16 typeField, QVariant field)
{
	switch (typeField) 
	{
	case ChangeNickname:
		user->setNickname(field.value<QString>());
		break;

	case ChangeIcon:
		user->setIcon(field.value<QImage>());
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


void addToIndex(QSharedPointer<Document> doc)
{
	QFile file(INDEX_FILENAME);
	if (file.open(QIODevice::Append | QIODevice::Text))
	{
		QTextStream indexFileStream(&file);

		indexFileStream << doc->getURI() << endl;

		if (indexFileStream.status() == QTextStream::Status::WriteFailed)
		{
			// TODO: handle error, throw FileWriteException ?
		}

		file.close();
	}
	else
	{
		QFileInfo info(file);
		throw FileWriteException(info.absolutePath().toStdString(), info.fileName().toStdString());
	}
}


/* create a new worskpace, a new thread and bind the workspace's affinity the the thread*/
WorkSpace* TcpServer::createNerWorkspace(QSharedPointer<Document> document, QString uri, QSharedPointer<Client> client)
{
	WorkSpace* w = new WorkSpace(document, QSharedPointer<TcpServer>(this));
	//QSharedPointer<WorkSpace> w = QSharedPointer<WorkSpace>(new WorkSpace(document, QSharedPointer<TcpServer>(this)));
	QThread* t = new QThread();

	documents.insert(uri, document);
	workThreads.insert(uri, QSharedPointer<QThread>(t));

	/* change affinity of this workspace with a new thread */
	w->moveToThread(t);
	t->start();

	/* workspace will notify when theres no one using it for delete */
	connect(w, &WorkSpace::notWorking, this, &TcpServer::deleteWorkspace);

	return w;
}

/* create a new Document */
bool TcpServer::createNewDocument(QString documentName, QString uri, QTcpSocket* author)
{
	/* check if documents is already used */
	if (documents.contains(uri))
		return false;

	QSharedPointer<Client> c = clients.find(author).value();
	QSharedPointer<Document> doc(new Document(uri));
	WorkSpace* w = createNerWorkspace(doc, uri, c);
	QFile docFile(uri);
	
	/* Add the document to the index, create the document file and update internal data structures */
	addToIndex(doc);
	docFile.open(QIODevice::NewOnly);
	docFile.close();
	
	/* add to client list the newDocument*/
	c->getUser()->addDocument(uri);
	/* add to document list the new editor */
	doc->insertNewEditor(c->getUserName());

	/* this thread will not receives more messages from client */
	disconnect(author, &QTcpSocket::readyRead, this, &TcpServer::readMessage);	
	disconnect(author, &QTcpSocket::disconnected, this, &TcpServer::clientDisconnection);

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

	QSharedPointer<Client> c = clients.find(client).value();
	QSharedPointer<Document> d = documents.find(uri).value();
	WorkSpace* w = nullptr;

	/* check if this documents is already load in memory or not */
	if (!workspaces.contains(uri)) {	/* need to be load */
		w = createNerWorkspace(d, uri, c);
	}
	else {								/* already load */
		w = workspaces.find(uri).value().get();
	}

	/* add to client list the newDocument*/
	c->getUser()->addDocument(uri);
	/* add to document list the new editor */
	d->insertNewEditor(c->getUserName());

	/* this thread will not recives more messages from client */
	disconnect(client, &QTcpSocket::readyRead, this, &TcpServer::readMessage);
	disconnect(client, &QTcpSocket::disconnected, this, &TcpServer::clientDisconnection);

	/* make the new thead connect the socket in the workspace */
	connect(this, &TcpServer::newSocket, w, &WorkSpace::newSocket);
	emit newSocket(static_cast<qint64>(client->socketDescriptor()));
	disconnect(this, &TcpServer::newSocket, w, &WorkSpace::newSocket);

	return true;
}

/* delete a document from a client */
bool TcpServer::removeDocument(QString uri, QTcpSocket* client)
{
	if (!documents.contains(uri))
		return false;

	if (!workspaces.contains(uri)) {
		return false;
	}

	clients.find(client).value()->getUser()->removeDocument(uri);

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

	// TODO: write on disk the file
	WorkSpace* w = workspaces.find(document).value().get();
	QThread* t = workThreads.find(document).value().get();

	/* they throw an exception ??? why??? */
	//disconnect(w, &WorkSpace::notWorking, this, &TcpServer::deleteWorkspace);
	//disconnect(w, &WorkSpace::deleteClient, this, &TcpServer::deleteClient);

	/* delete workspace and remove it from the map */
	workspaces.remove(document);	

	/* stop the thread, wait for exit, delete thread and remove it from the map */
	workThreads.find(document).value()->quit();
	workThreads.find(document).value()->wait();
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
			msg = std::make_unique<DocumentMessage>(NewDocument, streamIn, clients.find(socket).value()->getUserName());
			break;

		case OpenDocument:
			msg = std::make_unique<DocumentMessage>(OpenDocument, streamIn, clients.find(socket).value()->getUserName());
			break;

		case DocumentRemove:
			msg = std::make_unique<DocumentMessage>(DocumentRemove, streamIn, clients.find(socket).value()->getUserName());
			break;

		case UriRequest:
			msg = std::make_unique<DocumentMessage>(UriRequest, streamIn, clients.find(socket).value()->getUserName());
			break;

		default:
			throw MessageUnknownTypeException(msg->getType());
			break;
		}

		handleMessage(std::move(msg), socket);
	}
	catch (MessageUnknownTypeException& e) {
		QDataStream streamOut;
		streamOut.setDevice(socket);
		QString err = e.what();
		streamOut << (quint16)WrongMessageType << msg->getType();
	}
	catch (FieldWrongException& e) {
		/* send to the client WrongFieldType + message */
		QDataStream streamOut;
		streamOut.setDevice(socket);
		QString err = e.what();
		streamOut << (quint16)WrongFieldType << err;
	}
	catch (UserNotFoundException& e) {
		/* send by TcpServer::getUriFromUser */
	}
}

/* handle the message create by readMessage */
void TcpServer::handleMessage(std::unique_ptr<Message>&& msg, QTcpSocket* socket)
{
	QDataStream streamOut;
	quint16 typeOfMessage = 0;
	QString msg_str;

	streamOut.setDevice(socket); /* connect stream with socket */

	switch (msg->getType()) {

		/* Login */
	case LoginRequest:
	{
		LoginMessage* loginRequest = dynamic_cast<LoginMessage*>(msg.get());
		qDebug() << " -- si e' collegato: " << loginRequest->getUserName();

		if (!users.contains(loginRequest->getUserName()))
		{
			typeOfMessage = LoginError;
			msg_str = "Wrong username/password";
			streamOut << typeOfMessage << msg_str;
		}
		else
			sendLoginChallenge(socket, loginRequest->getUserName());
		break;
	}
		
	case LoginUnlock:
	{
		LoginMessage* loginUnlock = dynamic_cast<LoginMessage*>(msg.get());
		QString username = clients.find(socket).value()->getUserName();
		User u;

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
		
		if (!clients.find(socket).value()->isLogged()) {
			typeOfMessage = AccountDenied;
			msg_str = "client not logged";
		}
		else if (!updateAccount(clients.find(socket).value()->getUser(), accntUpdate->getFieldType(), accntUpdate->getField()))
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

		if (!clients.find(socket).value()->isLogged()) {
			typeOfMessage = DocumentError;
			msg_str = "client not logged";
		}
		else if (!createNewDocument(newDocument->getDocName(), newDocument->getURI(), socket)) 
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

		if (!clients.find(socket).value()->isLogged()) {
			typeOfMessage = DocumentError;
			msg_str = "client not logged";
		}
		else if (!openDocument(docMsg->getURI(), socket)) {
			typeOfMessage = DocumentError;
			msg_str = "Cannot open '" + docMsg->getDocName() + "', this document doesn't exist";
		}
		else {
			typeOfMessage = DocumentOpened;
			msg_str = "Document opened";
		}
		streamOut << typeOfMessage << msg_str;
		break;
	}

	case DocumentRemove:
	{
		DocumentMessage* docMsg = dynamic_cast<DocumentMessage*>(msg.get());

		if (!clients.find(socket).value()->isLogged()) {
			typeOfMessage = DocumentError;
			msg_str = "client not logged";
		}
		else if (!removeDocument(docMsg->getURI(), socket)) {
			typeOfMessage = DocumentError;
			msg_str = "Cannot delete '" + docMsg->getDocName() + "', this document doesn't exist";
		}
		else {
			typeOfMessage = DocumentOpened;
			msg_str = "Document deleted";
		}
		streamOut << typeOfMessage << msg_str;
		break;
	}

	case UriRequest:
	{
		DocumentMessage* uriRequest = dynamic_cast<DocumentMessage*>(msg.get());

		if (!clients.find(socket).value()->isLogged()) {
			typeOfMessage = DocumentError;
			msg_str = "client not logged";
			streamOut << typeOfMessage << msg_str;
		}
		else {
			typeOfMessage = UriResponse;
			streamOut << typeOfMessage << getUriFromUser(uriRequest->getUserName());
		}
		break;
	}


	default:
	{
		typeOfMessage = WrongMessageType;
		streamOut << typeOfMessage << msg->getType();
		break;
	}
		
	}
}


/****************************** SERVER METHODS ******************************/


/* allow to a secondary thread to steal a QSharedPointer from the server */
QSharedPointer<Client> TcpServer::moveClient(qintptr socketDescriptor)	// TODO: remove workspace param, Client is not QObject
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

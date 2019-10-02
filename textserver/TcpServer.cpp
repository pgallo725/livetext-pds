#include "TcpServer.h"

#include <iostream>

#include <QFileInfo>
#include <QDataStream>
#include <QtNetwork>
#include <QHostAddress>
#include <QDateTime>
#include <QRandomGenerator>
#include <QDir>

#include <MessageFactory.h>
#include "ServerException.h"
#include "SharedException.h"

#define INDEX_FILENAME "./Documents/documents.dat"
#define USERS_FILENAME "users.dat"
#define TMP_USERS_FILENAME "users.tmp"


/* Server costructor */
TcpServer::TcpServer(QObject* parent)
	: QTcpServer(parent), messageHandler(this), _userIdCounter(0), 
	usersFile(QSaveFile(USERS_FILENAME)), docsFile(QSaveFile(INDEX_FILENAME))
{
	qRegisterMetaType<QSharedPointer<Client>>("QSharedPointer<Client>");
	qRegisterMetaType<URI>("URI");
	qRegisterMetaType<MessageCapsule>("MessageCapsule");

	/* initialize random number generator with timestamp */
	qsrand(QDateTime::currentDateTime().toTime_t());

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
		qDebug() << "Qt built with SSL version: " << QSslSocket::sslLibraryBuildVersionString();
		qDebug() << "SSL version supported on this system: " << QSslSocket::supportsSsl() << endl;

		QSslCipher ECDHE_RSA_cipher;
		for (QSslCipher cipher : QSslConfiguration::supportedCiphers())
		{
			if (cipher.name() == "ECDHE-RSA-AES256-GCM-SHA384")
			{
				ECDHE_RSA_cipher = cipher;
				break;
			}
		}

		if (ECDHE_RSA_cipher.isNull())
		{
			qDebug() << "Server does not support the requested SSL cipher";
			return;
		}

		config.setCiphers(QList<QSslCipher>{ ECDHE_RSA_cipher });
		qDebug() << "Preferred SSL ciphers: " << config.ciphers() << endl;

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
	qDebug() << "Server down";
}

/* Load users and documents */
void TcpServer::initialize()
{
	if (!QFileInfo(QFile("server.key")).exists()) {
		throw ServerStartException("Cannot find 'server.key' file");
	}
	if (!QFileInfo(QFile("server.pem")).exists()) {
		throw ServerStartException("Cannot find 'server.pem' file");
	}
	
	// Open the file and read the users database
	QFile usersFile(USERS_FILENAME);
	if (usersFile.open(QIODevice::ReadWrite))
	{
		std::cout << "\nLoading users database... ";

		QDataStream usersDbStream(&(usersFile));

		// Load the users database in the server's memory
		// using built-in Qt Map deserialization
		usersDbStream >> users;

		if (usersDbStream.status() == QTextStream::ReadCorruptData)
		{
			throw FileLoadException(USERS_FILENAME);
		}

		usersFile.close();

		std::cout << "done" << std::endl;
	}
	else
	{
		throw FileOpenException(USERS_FILENAME, QDir::currentPath().toStdString());
	}

	// Initialize the counter to assign user IDs
	_userIdCounter = users.size();

	if (!QDir("Documents").exists()) {
		if (!QDir().mkdir("Documents")) {
			throw ServerStartException("Cannot create 'Documents' folder");
		}
	}

	// Read the documents' index file
	QFile docsFile(INDEX_FILENAME);
	if (docsFile.open(QIODevice::ReadWrite| QIODevice::Text))
	{
		std::cout << "\nLoading documents index file... ";

		QTextStream docIndexStream(&docsFile);

		// Load the document index in the server's memory
		while (!docIndexStream.atEnd())
		{
			QString docURI;

			docIndexStream >> docURI;

			if (docIndexStream.status() == QTextStream::ReadCorruptData)
			{
				throw FileLoadException(INDEX_FILENAME);
			}
			if (!docURI.isEmpty())
			{
				documents.insert(docURI, QSharedPointer<Document>(new Document(docURI)));
			}
		}

		docsFile.close();
		std::cout << "done" << std::endl;
	}
	else
	{
		throw FileOpenException(INDEX_FILENAME, QDir::currentPath().toStdString());
	}

	std::cout << "\nServer up" << std::endl;
}

/* Generate the URI for a document */
URI TcpServer::generateURI(QString authorName, QString docName) const
{
	QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
	QString str = authorName + "_" + docName + "_";

	// Initialize the random number generator with a sequence of characters composed by
	// author username + document name, so that any user is required to have documents with unique names
	std::string seedStr = str.toStdString();
	std::seed_seq seed(seedStr.begin(), seedStr.end());
	QRandomGenerator randomizer(seed);

	for (int i = 0; i < 12; ++i)	// add a 12-character long random sequence to the document URI to make it unique
	{
		quint32 index = randomizer.bounded(possibleCharacters.length());
		QChar nextChar = possibleCharacters.at(index);
		str.append(nextChar);
	}

	return URI(str);
}

/* Save users list on persistent storage */
void TcpServer::saveUsers()
{
	// Create the new users database file and write the data to it
	//QSaveFile file(USERS_FILENAME);

	if (usersFile.open(QIODevice::WriteOnly)) {
		QDataStream usersDb(&usersFile);

		std::cout << "\nSaving users database... ";

		// Write the the current users informations to file
		// using built-in Qt Map serialization
		usersDb << users;
		
		// Check datastream status
		if (usersDb.status() == QTextStream::Status::WriteFailed)
		{
			usersFile.cancelWriting();
			usersFile.commit();
			throw FileWriteException(USERS_FILENAME, QDir::currentPath().toStdString());
		}
		usersFile.commit();
		std::cout << "done" << std::endl;
	}
	else
	{
		throw FileCreateException(USERS_FILENAME, QDir::currentPath().toStdString());
	}
}

/* Handle a new connection from a client */
void TcpServer::newClientConnection()
{
	/* need to grab the socket - socket is created as a child of server */
	QSslSocket* socket = dynamic_cast<QSslSocket*>(this->nextPendingConnection());

	/* check if there's a new connection or it was a windows signal */
	if (socket == nullptr) {
		qDebug() << "WARNING: received fake connection signal from Windows\n";
		return;
	}

	qDebug() << " - new connection from a client";

	QSharedPointer<Client> client(new Client(socket));
	clients.insert(socket, client);

	/* connect slots to be able to read messages */
	connect(socket, &QSslSocket::readyRead, this, &TcpServer::readMessage);
	connect(socket, &QSslSocket::disconnected, this, &TcpServer::clientDisconnection);
}

/* Handle client disconnection and release resources */
void TcpServer::clientDisconnection()
{
	QSslSocket* socket = dynamic_cast<QSslSocket*>(sender());

	qDebug() << " - client disconnected";

	restoreUserAvaiable(clients.find(socket).value()->getUsername());
	clients.remove(socket);					/* remove this client from the map */
	socket->close();						/* close and destroy the socket */
	socket->deleteLater();
}


/****************************** SSL METHODS ******************************/


void TcpServer::socketErr(QAbstractSocket::SocketError socketError)
{
	qDebug() << "Socket error: " << socketError << endl;
}

void TcpServer::ready()
{
	qDebug() << "Server encryption setup completed" << endl;
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
	QSslSocket* serverSocket = new QSslSocket;
	connect(serverSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &TcpServer::socketErr);

	if (serverSocket->setSocketDescriptor(socketDescriptor))
	{
		serverSocket->setSslConfiguration(config);
		serverSocket->setPrivateKey("server.key", QSsl::Rsa, QSsl::Pem, "LiveTextKey");
		serverSocket->setLocalCertificate("server.pem", QSsl::Pem);

		qDebug() << "Created new SSL socket with certificate " << serverSocket->localCertificate() << endl;;

		addPendingConnection(serverSocket);
		connect(serverSocket, &QSslSocket::encrypted, this, &TcpServer::ready);
		serverSocket->startServerEncryption();
	}
	else
	{
		delete serverSocket;
	}
}


/****************************** ACCOUNT METHODS ******************************/


/* create a new client and send nonce to be solved for authentication */
MessageCapsule TcpServer::serveLoginRequest(QSslSocket* clientSocket, QString username)
{
	qDebug() << "si e' collegato: " << username;

	QSharedPointer<Client> client = clients.find(clientSocket).value();

 	if (users.contains(username))
	{
		if (client->isLogged())
			return MessageFactory::LoginError("Your client is already logged in as '" + client->getUsername() + "'");

		if(usersNotAvaiable.contains(username))
			return MessageFactory::LoginError("This user is already logged in ");

		return MessageFactory::LoginChallenge(users.find(username).value().getSalt(),
			client->challenge(&(users.find(username).value())));
	}
	else return MessageFactory::LoginError("The specified username is not registered on the server");
}

/* Authenticate the client's User and apply the login */
MessageCapsule TcpServer::authenticateUser(QSslSocket* clientSocket, QByteArray token)
{
	QSharedPointer<Client> client = clients.find(clientSocket).value();

	if (client->isLogged())
		return MessageFactory::LoginError("You are already logged in");

	if (usersNotAvaiable.contains(client->getUsername()))
		return MessageFactory::LoginError("This user is already logged in ");

	if (client->authentication(token))
	{
		usersNotAvaiable << client->getUsername();
		client->login(client->getUser());
		return MessageFactory::LoginGranted(*client->getUser());
	}
	else
	{
		client->logout();
		return MessageFactory::LoginError("Wrong username/password");
	}
}

/* Create a new User and register it on the server */
MessageCapsule TcpServer::createAccount(QSslSocket* socket, QString username, QString nickname, QImage icon, QString password)
{
	QSharedPointer<Client> client = clients.find(socket).value();
	if (client->isLogged())
		return MessageFactory::AccountError("You cannot create an account while being logged in as another user");

	/* check if username or password are nulls */
	if (!username.compare("") || !password.compare(""))
		return MessageFactory::AccountError("Username and/or password cannot be empty");

	/* check if this username is already used */
	if (users.contains(username))
		return MessageFactory::AccountError("That username is already taken");
	
	User user(username, _userIdCounter++, nickname, password, icon);		/* create a new user		*/
	QMap<QString, User>::iterator i = users.insert(username, user);			/* insert new user in map	*/

	client->login(&(*i));		// client is automatically logged

	try {
		saveUsers();
	}
	catch (FileException& fe) {
		client->logout();
		users.remove(username);
		return MessageFactory::AccountError("Cannot create new account for internal problem, plese try later");
	}
	
	return MessageFactory::AccountConfirmed(user);
}

/* Check and update user's fields and return response message for the client in tcpServer */
MessageCapsule TcpServer::updateAccount(QSslSocket* clientSocket, QString nickname, QImage icon, QString password)
{
	Client* client = clients.find(clientSocket).value().get();
	User recoveryUser = *(client->getUser());

	if (!client->isLogged())
		return MessageFactory::AccountError("You are not logged in");

	MessageCapsule msg = accountUpdate(client->getUser(), nickname, icon, password);
	
	try {
		saveUsers();
	}
	catch (FileException & fe) {
		client->getUser()->recoveryUser(recoveryUser);
		return MessageFactory::AccountError("Cannot update the account for internal problem, plese try later");
	}
	
	return msg;
}

/* Update user's fields */
MessageCapsule TcpServer::accountUpdate(User* user, QString nickname, QImage icon, QString password)
{
	if (!nickname.isEmpty())
		user->setNickname(nickname);
	if (!icon.isNull())
		user->setIcon(icon);
	if (!password.isEmpty())
		user->setPassword(password);

	return MessageFactory::AccountConfirmed(*user);
}

/* Changes the state of a Client object to "logged out" */
void TcpServer::logoutClient(QSslSocket* clientSocket)
{
	QSharedPointer<Client> c = clients.find(clientSocket).value();
	c->logout();
	restoreUserAvaiable(c->getUsername());
}

/* Move a client from the workspace that he has exited back to the server */
void TcpServer::receiveClient(QSharedPointer<Client> client)
{
	/* get the client's socket and bring it back to the server thread */
	QSslSocket* socket = client->getSocket();
	socket->setParent(this);

	clients.insert(socket, client);

	/* reconnect socket signals to slots in order to read and handle messages */
	connect(socket, &QSslSocket::readyRead, this, &TcpServer::readMessage);
	connect(socket, &QSslSocket::disconnected, this, &TcpServer::clientDisconnection);

	socket->readAll();
}

/* Check and update user's fields and return response message for the client in workSpace */
void TcpServer::receiveUpdateAccount(QSharedPointer<Client> client, QString nickname, QImage icon, QString password)
{
	WorkSpace* w = dynamic_cast<WorkSpace*>(sender());
	connect(this, &TcpServer::sendAccountUpdate, w, &WorkSpace::receiveUpdateAccount);
	if (!client->isLogged())
		emit sendAccountUpdate(client, MessageFactory::AccountError("You are not logged in"));
	else
	{
		User recoveryUser = *(client->getUser());
		MessageCapsule msg = accountUpdate(client->getUser(), nickname, icon, password);
		try {
			saveUsers();
		}
		catch (FileException& fe) {
			client->getUser()->recoveryUser(recoveryUser);
			emit sendAccountUpdate(client, MessageFactory::AccountError("Cannot update the account for internal problem, plese try later"));
		}
		emit sendAccountUpdate(client, msg);
	}
	disconnect(this, &TcpServer::sendAccountUpdate, w, &WorkSpace::receiveUpdateAccount);
}

/* Delete user from UnAvaiable list when they logout or close connection */
void TcpServer::restoreUserAvaiable(QString username)
{
	usersNotAvaiable.removeOne(username);
}



/****************************** DOCUMENT METHODS ******************************/


void TcpServer::saveDocIndex()
{
	if (docsFile.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream indexFileStream(&docsFile);

		for (URI uri : documents.keys())
			indexFileStream << uri.toString() << endl;
		
		if (indexFileStream.status() == QTextStream::Status::WriteFailed)
		{
			docsFile.cancelWriting();
			docsFile.commit();
			throw FileWriteException(INDEX_FILENAME, QDir::currentPath().toStdString());
		}
	}
	else
	{
		throw FileOpenException(INDEX_FILENAME, QDir::currentPath().toStdString());
	}
}


/* Create a new worskpace for a document */
QSharedPointer<WorkSpace> TcpServer::createWorkspace(QSharedPointer<Document> document, QSharedPointer<Client> client)
{
	QSharedPointer<WorkSpace> w = QSharedPointer<WorkSpace>(new WorkSpace(document));
	documents.insert(document->getURI(), document);
	workspaces.insert(document->getURI(), w);
	
	/* workspace will notify when clients quit editing the document and when it becomes empty */
	connect(w.get(), &WorkSpace::returnClient, this, &TcpServer::receiveClient);
	connect(w.get(), &WorkSpace::noEditors, this, &TcpServer::deleteWorkspace);
	connect(w.get(), &WorkSpace::restoreUserAvaiable, this, &TcpServer::restoreUserAvaiable, Qt::QueuedConnection);
	connect(w.get(), &WorkSpace::sendAccountUpdate, this, &TcpServer::receiveUpdateAccount, Qt::QueuedConnection);
	
	return w;
}

/* Create a new Document */
MessageCapsule TcpServer::createDocument(QSslSocket* author, QString docName)
{
	QSharedPointer<Client> client = clients.find(author).value();
	User recoveryUser = *(client->getUser());

	if (!client->isLogged())
		return MessageFactory::DocumentError("You are not logged in");

	URI docURI = generateURI(client->getUsername(), docName);

	/* check if the document URI is unique */
	if (documents.contains(docURI))
		return MessageFactory::DocumentError("A document with the same URI already exists");

	QSharedPointer<Document> doc(new Document(docURI, client->getUserId()));
	QSharedPointer<WorkSpace> w;

	try {
		w = createWorkspace(doc, client);
	}
	catch (DocumentException & de) {
		return MessageFactory::DocumentError("Cannot create new document for internal problem, please try later");
	}
	
	/* add to newly created document to those owned by the user */
	client->getUser()->addDocument(docURI);

	try {
		/* add the document to the index and save the file */
		saveDocIndex();
		saveUsers();
		docsFile.commit();
	}
	catch (FileOpenException& foe) {
		client->getUser()->recoveryUser(recoveryUser);
		documents.remove(doc->getURI());
		return MessageFactory::DocumentError("Cannot create new document for internal problem, please try later");
	}
	catch (FileException& fe) {
		docsFile.cancelWriting();
		docsFile.commit();
		client->getUser()->recoveryUser(recoveryUser);
		documents.remove(doc->getURI());
		return MessageFactory::DocumentError("Cannot create new document for internal problem, please try later");
	}

	/* add this user to the list of document editors */
	doc->insertNewEditor(client->getUsername());

	/* this thread will not receives more messages from client */
	disconnect(author, &QSslSocket::readyRead, this, &TcpServer::readMessage);
	disconnect(author, &QSslSocket::disconnected, this, &TcpServer::clientDisconnection);
	disconnect(author, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &TcpServer::socketErr);

	/* move the socket's affinity to the workspace thread */
	QSslSocket* s = client->getSocket();
	s->setParent(nullptr);
	s->moveToThread(w->thread());

	clients.remove(author);		// remove the Client from the server map

	/* move the client object from the server to the workspace thread */
	connect(this, &TcpServer::clientToWorkspace, w.get(), &WorkSpace::newClient);
	emit clientToWorkspace(std::move(client));
	disconnect(this, &TcpServer::clientToWorkspace, w.get(), &WorkSpace::newClient);

	return MessageCapsule();
}

/* Open an existing Document */
MessageCapsule TcpServer::openDocument(QSslSocket* clientSocket, URI docUri)
{
	QSharedPointer<Client> client = clients.find(clientSocket).value();
	User recoveryUser = *(client->getUser());

	if (!client->isLogged())
		return MessageFactory::DocumentError("You are not logged in");

	/* check if this document doesn't exist */
	if (!documents.contains(docUri))
		return MessageFactory::DocumentError("The requested document does not exist (invalid URI)");

	QSharedPointer<Document> doc = documents.find(docUri).value();
	QSharedPointer<WorkSpace> w;

	try
	{
		// check if the documents exist
		if (!doc->exist()) {
			// if the doc doesn't exist docUri is removed from documents map and from user documents
			documents.remove(docUri);
			client->getUser()->removeDocument(docUri);

			saveDocIndex();
			saveUsers();
			docsFile.commit();

			return MessageFactory::DocumentError("This document doesn't exist");
		}

		w = workspaces.contains(docUri) ?
			workspaces.find(docUri).value() :
			createWorkspace(doc, client);
	}
	catch (FileException & fe) {
		docsFile.cancelWriting();
		docsFile.commit();
		return MessageFactory::DocumentError("Cannot open the document for internal problem, please try later");
	}
	catch (DocumentException & de)
	{
		return MessageFactory::DocumentError("Cannot open the document for internal problem, please try later");
	}

	User* user = client->getUser();

	/* if it's the first time opening this document, add it to the user's list of documents */	
	if (!user->hasDocument(docUri)) {
		user->addDocument(docUri);
		try {
			saveUsers();
		}
		catch (FileException& fe) {
			client->getUser()->recoveryUser(recoveryUser);
			return MessageFactory::DocumentError("Cannot open the document for internal problem, please try later");
		}
	}
	/* and add the new editor to the document's list of editors */
	doc->insertNewEditor(user->getUsername());

	/* this thread will not recives more messages from client */
	disconnect(clientSocket, &QSslSocket::readyRead, this, &TcpServer::readMessage);
	disconnect(clientSocket, &QSslSocket::disconnected, this, &TcpServer::clientDisconnection);
	disconnect(clientSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &TcpServer::socketErr);

	/* move the socket's affinity to the workspace thread */
	QSslSocket* s = client->getSocket();
	s->setParent(nullptr);
	s->moveToThread(w->thread());

	clients.remove(clientSocket);		// remove the Client from the server map

	/* move the client object from the server to the workspace thread */
	connect(this, &TcpServer::clientToWorkspace, w.get(), &WorkSpace::newClient);
	emit clientToWorkspace(client);
	disconnect(this, &TcpServer::clientToWorkspace, w.get(), &WorkSpace::newClient);

	return MessageCapsule();
}

/* Delete a document from the client's list */
MessageCapsule TcpServer::removeDocument(QSslSocket* clientSocket, URI docUri)
{
	QSharedPointer<Client> client = clients.find(clientSocket).value();

	if (!client->isLogged())
		return MessageFactory::DocumentError("You are not logged in");

	if (!documents.contains(docUri))
		return MessageFactory::DocumentError("The specified document does not exist (invalid URI)");

	client->getUser()->removeDocument(docUri);
	return MessageFactory::DocumentDismissed();
}


/* Release all resources owned by workspace and delete it */
void TcpServer::deleteWorkspace(URI document)
{
	/* delete workspace and remove it from the map */
	try {
		workspaces.remove(document);
	}
	catch (DocumentException& de)
	{
		//TODO: who handle?
	}
		
	qDebug() << "workspace (" << document.toString() << ") closed";
}


/****************************** MESSAGES METHODS ******************************/


/* Read the message from the socket and create the correct type message for the handler */
void TcpServer::readMessage()
{
	QSslSocket* socket = dynamic_cast<QSslSocket*>(sender());
	QDataStream streamIn(socket);	/* connect stream with socket */
	QByteArray dataBuffer;

	if (!socketBuffer.getDataSize()) {
		streamIn >> socketBuffer;
	}

	dataBuffer = socket->read((qint64)(socketBuffer.getDataSize() - socketBuffer.getReadDataSize()));	// Read all the available message data from the socket

	socketBuffer.append(dataBuffer);

	if (socketBuffer.bufferReadyRead()) {
		
		QDataStream dataStream(&(socketBuffer.buffer), QIODevice::ReadWrite);
		quint16 mType = socketBuffer.getType();
		MessageCapsule message = MessageFactory::Empty((MessageType)mType);
		message->readFrom(dataStream);

		socketBuffer.clear();

		if (mType == LoginRequest || mType == LoginUnlock || mType == AccountCreate || mType == AccountUpdate ||
			mType == Logout || mType == DocumentCreate || mType == DocumentOpen || mType == DocumentRemove)
		{
			messageHandler.process(message, socket);
		}
		else
		{
			message = MessageFactory::Failure(QString("Unknown message type : ") + QString::number(mType));
			message->send(socket);
		}
	}
}


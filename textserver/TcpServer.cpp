#include "TcpServer.h"

#include <QtNetwork>
#include <QHostAddress>
#include <QDataStream>
#include <QDateTime>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QFileInfo>
#include <QDir>

#include <MessageFactory.h>
#include <SharedException.h>


/* Server constructor */
TcpServer::TcpServer(QObject* parent)
	: QTcpServer(parent), messageHandler(this), _userIdCounter(0)
{
	qRegisterMetaType<QSharedPointer<Client>>("QSharedPointer<Client>");
	qRegisterMetaType<URI>("URI");
	qRegisterMetaType<MessageCapsule>("MessageCapsule");

	qInfo().nospace() << "LiveText Server (version 0.9.2)" << endl
		<< "Politecnico di Torino - a.a. 2018/2019 " << endl;

	/* initialize random number generator with timestamp */
	qsrand(QDateTime::currentDateTime().toTime_t());

	/* connect newConnection from QTcpServer to newClientConnection() */
	connect(this, &QTcpServer::newConnection, this, &TcpServer::newClientConnection);

	/* search for a free port */
	for (quint16 i = 1500; i < 10000; i += 101) {

		/* server listen on 0.0.0.0::i - return true on success */
		if (this->listen(QHostAddress::Any, i)) {
			break;
		}
	}

	if (!this->isListening())
	{
		throw StartupException("TcpServer could not find an available port to listen on");
	}
	else
	{
		qInfo() << "Qt built with SSL version: " << QSslSocket::sslLibraryBuildVersionString();
		qInfo() << "SSL version supported on this system: " << QSslSocket::supportsSsl() << endl;

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
			throw StartupException("The system does not support the requested SSL cipher (ECDHE-RSA-AES256-GCM-SHA384)");
		}
		else config.setCiphers(QList<QSslCipher>{ ECDHE_RSA_cipher });

		/* Get IP address and port */
		QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
		QString ip_address = this->serverAddress().toString();
		quint16 port = this->serverPort();
		if (this->isListening()) {
			qInfo() << "Reachable addresses: ";
			for (const QHostAddress& address : ipAddressesList) {
				if (address.protocol() == QAbstractSocket::IPv4Protocol)
					qInfo().noquote() << " - " << address.toString();
			}
			qInfo() << endl << "Available on TCP/IP port:" << port << endl << endl;
		}
	}
}

/* Destructor */
TcpServer::~TcpServer()
{
	// All resources are held by smart pointers, which take care of releasing them
	// automatically when the TcpServer object is destroyed
}

/* Load users and documents */
void TcpServer::initialize()
{
	qDebug() << "> BEGIN SERVER INITIALIZATION PROCEDURE...";
	qDebug() << "> Checking SSL resources availability";


	if (!QFileInfo(QFile("server.key")).exists()) {
		throw StartupException("Cannot find private key file: 'server.key'");
	}
	if (!QFileInfo(QFile("server.pem")).exists()) {
		throw StartupException("Cannot find local certificate file: 'server.pem'");
	}

	// Create a connection to the server's database
	qDebug() << "> Opening connection to server database";
	db.open("livetext.db3");
	qDebug() << "> (COMPLETED)";


	// Loading the documents index in the server memory
	qDebug() << "> Loading documents index";
	foreach(QString docURI, db.readDocumentURIs())
	{
		if (validateURI(docURI))
			documents.insert(docURI, QSharedPointer<Document>(new Document(docURI)));
		else {
			db.removeDoc(docURI);
			qDebug() << "> Invalid URI" << docURI << "skipped and removed";
		}
	}
	qDebug() << "> (COMPLETED)";

	qDebug() << "> Loading users database";

	for each (User user in db.readUsersList())
	{
		for each (URI docUri in db.readUserDocuments(user.getUsername()))
		{
			user.addDocument(docUri);
		}
		users.insert(user.getUsername(), user);
	}

	qDebug() << "> (COMPLETED)";

	// Initialize the counter to assign user IDs
	_userIdCounter = db.getMaxUserID();

	// Check existence of (or create) the Documents folder
	if (!QDir("Documents").exists()) 
	{
		qDebug() << "> Creating the server Documents folder";
		if (!QDir().mkdir("Documents")) {
			throw StartupException("Cannot create 'Documents' folder");
		}
	}

	qDebug() << "> (INITIALIZATION COMPLETE)" << endl;
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

/* Verifies if an URI satisfies all format constraints to be considered valid */
bool TcpServer::validateURI(URI uri) const
{
	QRegularExpression uriFormat("^[^_]+_[^_]+_[a-zA-Z0-9]{12}$");

	// Check if the candidate URI has the correct format
	if (!uriFormat.match(uri.toString()).hasMatch())
		return false;

	// Check the correctness of the trailing hash sequence
	return uri == generateURI(uri.getAuthorName(), uri.getDocumentName());
}

/* Handle a new connection from a client */
void TcpServer::newClientConnection()
{
	/* need to grab the socket - socket is created as a child of server */
	QSslSocket* socket = dynamic_cast<QSslSocket*>(this->nextPendingConnection());

	/* check if there's a new connection or it was a windows signal */
	if (socket == nullptr) {
		qDebug() << "> (WARNING) Received fake connection signal from Windows\n";
		return;
	}

	qDebug() << "> New connection from a client";

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
	QSharedPointer<Client> client = clients.find(socket).value();

	if (client->isLogged())
	{
		qDebug() << "> Client" << client->getUsername() << "disconnected";
		restoreUserAvaiable(client->getUsername());
	}

	clients.remove(socket);					/* remove this client from the map */
	socket->close();						/* close and destroy the socket */
	socket->deleteLater();
}


/****************************** SSL METHODS ******************************/


void TcpServer::sslSocketError(QAbstractSocket::SocketError error)
{
	if (error != QAbstractSocket::RemoteHostClosedError)
		qDebug() << "> (ERROR) SslSocket error: " << error ;
}

void TcpServer::sslSocketReady()
{
	qDebug() << "> SslSocket handshake successful, encryption setup completed";
}

void TcpServer::incomingConnection(qintptr socketDescriptor)
{
	QSslSocket* serverSocket = new QSslSocket;
	connect(serverSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &TcpServer::sslSocketError);

	if (serverSocket->setSocketDescriptor(socketDescriptor))
	{
		serverSocket->setSslConfiguration(config);
		serverSocket->setPrivateKey("server.key", QSsl::Rsa, QSsl::Pem, "LiveTextKey");
		serverSocket->setLocalCertificate("server.pem", QSsl::Pem);

		addPendingConnection(serverSocket);
		connect(serverSocket, &QSslSocket::encrypted, this, &TcpServer::sslSocketReady);
		serverSocket->startServerEncryption();
	}
	else
	{
		qDebug() << "> (ERROR) SslSocket creation failed, incoming connection was rejected";
		delete serverSocket;
	}
}


/****************************** USER ACCOUNT METHODS ******************************/


/* Create a new client and send nonce to be solved for authentication */
MessageCapsule TcpServer::serveLoginRequest(QSslSocket* clientSocket, QString username)
{
	QSharedPointer<Client> client = clients.find(clientSocket).value();

 	if (users.contains(username))
	{
		if (client->isLogged())
			return MessageFactory::LoginError("Your client is already logged in as '" + client->getUsername() + "'");

		if(usersNotAvailable.contains(username))
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

	if (usersNotAvailable.contains(client->getUsername()))
		return MessageFactory::LoginError("This user is already logged in ");

	if (client->authenticate(token))		// verify the user's account credentials
	{
		qDebug() << "> User" << client->getUsername() << "logged in";

		usersNotAvailable << client->getUsername();
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

	qDebug() << "> Creating new user account" << username;
	
	User user(username, _userIdCounter++, nickname, password, icon);		/* create a new user		*/
	QMap<QString, User>::iterator i = users.insert(username, user);			/* insert new user in map	*/

	client->login(&(*i));		// client is automatically logged in as the new user
	
	try {
		db.insertUser(user);
	}
	catch (DatabaseException & dbe) {
		qDebug().noquote() << ">" << "(DB ERROR) Cannot insert this new user: '" << user.getUsername() << "' - '" << user.getUserId() << "'";
		client->logout();
		users.remove(username);
		return MessageFactory::AccountError("Users database update failed, please try again later");
	}
	
	return MessageFactory::AccountConfirmed(user);
}

/* Check and update user's fields and return response message for the client in TcpServer */
MessageCapsule TcpServer::updateAccount(QSslSocket* clientSocket, QString nickname, QImage icon, QString password)
{
	Client* client = clients.find(clientSocket).value().get();
	User backupUser = *(client->getUser());

	if (!client->isLogged())
		return MessageFactory::AccountError("You are not logged in");

	qDebug() << "> Updating account information of user" << client->getUsername();

	User* user = client->getUser();
	user->update(nickname, icon, password);

	try {
		db.updateUser(*user);
	}
	catch (DatabaseException & dbe) {
		qDebug().noquote() << ">" << "(DB ERROR) Cannot update '" << user->getUsername();
		client->getUser()->rollback(backupUser);
		return MessageFactory::AccountError("Users database update failed, please try again later");
	}
	
	return MessageFactory::AccountConfirmed(*client->getUser());
}

/* Check and update user's fields and return response message for the client in workSpace */
void TcpServer::workspaceAccountUpdate(QSharedPointer<Client> client, QString nickname, QImage icon, QString password)
{
	WorkSpace* w = dynamic_cast<WorkSpace*>(sender());
	connect(this, &TcpServer::sendAccountUpdate, w, &WorkSpace::answerAccountUpdate);

	if (!client->isLogged())
		emit sendAccountUpdate(client, MessageFactory::AccountError("You are not logged in"));

	qDebug() << "> Updating account information of user" << client->getUsername() << "(inside Workspace)";

	User backupUser = *(client->getUser());
	User* user = client->getUser();
	user->update(nickname, icon, password);

	try {
		db.updateUser(*user);
	}
	catch (DatabaseException & dbe) {
		qDebug().noquote() << ">" << "(DB ERROR) Cannot update '" << user->getUsername();
		client->getUser()->rollback(backupUser);
		emit sendAccountUpdate(client, MessageFactory::AccountError("Users database update failed, please try again later"));
	}

	emit sendAccountUpdate(client, MessageFactory::AccountConfirmed(*client->getUser()));

	disconnect(this, &TcpServer::sendAccountUpdate, w, &WorkSpace::answerAccountUpdate);
}


/* Changes the state of a Client object to "logged out" */
void TcpServer::logoutClient(QSslSocket* clientSocket)
{
	QSharedPointer<Client> c = clients.find(clientSocket).value();
	QString username = c->getUsername();
	restoreUserAvaiable(username);
	c->logout();

	qDebug() << "> User" << username << "logged out";
}

/* Delete user from unavailable list when they logout or close connection */
void TcpServer::restoreUserAvaiable(QString username)
{
	usersNotAvailable.removeOne(username);
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
}


/****************************** DOCUMENT METHODS ******************************/

/* Create a new worskpace for a document */
QSharedPointer<WorkSpace> TcpServer::createWorkspace(QSharedPointer<Document> document)
{
	QSharedPointer<WorkSpace> w = QSharedPointer<WorkSpace>(new WorkSpace(document));
	workspaces.insert(document->getURI(), w);
	
	/* workspace will notify when clients quit editing the document and when it becomes empty */
	connect(w.get(), &WorkSpace::returnClient, this, &TcpServer::receiveClient);
	connect(w.get(), &WorkSpace::noEditors, this, &TcpServer::deleteWorkspace);
	connect(w.get(), &WorkSpace::userDisconnected, this, &TcpServer::restoreUserAvaiable, Qt::QueuedConnection);
	connect(w.get(), &WorkSpace::requestAccountUpdate, this, &TcpServer::workspaceAccountUpdate, Qt::QueuedConnection);
	
	return w;
}

/* Create a new Document */
MessageCapsule TcpServer::createDocument(QSslSocket* author, QString docName)
{
	QSharedPointer<Client> client = clients.find(author).value();

	if (!client->isLogged())
		return MessageFactory::DocumentError("You are not logged in");

	URI docURI = generateURI(client->getUsername(), docName);

	/* check if the document URI is unique */
	if (documents.contains(docURI))
		return MessageFactory::DocumentError("A document with the same URI already exists");

	qDebug() << "> Creating new document" << docURI.toString();

	// create a copy of the User object before it gets modified, for rollback support
	User* user = client->getUser();
	User backupUser = *(user);
	QSharedPointer<Document> doc;

	try 
	{	
		/* create and save the new document */
		doc = QSharedPointer<Document>(new Document(docURI, client->getUserId()));
		doc->save();	// (creates the document file)

		/* the user becomes the first editor of this document */
		documents.insert(doc->getURI(), doc);
		user->addDocument(doc->getURI());
		doc->insertNewEditor(user->getUsername());

		try {
			db.addDocToUser(user->getUsername(), docURI.toString());
		}
		catch (DatabaseException & dbe) {
			qDebug().noquote() << ">" << "(DB ERROR) Cannot insert: '" << user->getUsername() << " - " << docURI.toString();
			doc->remove();
			return MessageFactory::DocumentError("Document creation failed, please try again");
		}
	}
	catch (DocumentException& de) {
		doc->remove();
		return MessageFactory::DocumentError("Document creation failed, please try again");
	}

	qDebug() << "> (DOCUMENT CREATED)";

	return openDocument(author, docURI, true);
}

/* Open an existing Document */
MessageCapsule TcpServer::openDocument(QSslSocket* clientSocket, URI docUri, bool docJustCreated)
{
	QSharedPointer<Client> client = clients.find(clientSocket).value();
	QSharedPointer<WorkSpace> ws;

	if (!docJustCreated)		// Avoid these operations if we're being called by createDocument
	{
		if (!client->isLogged())
			return MessageFactory::DocumentError("You are not logged in");

		/* check if this document doesn't exist */
		if (!documents.contains(docUri))
			return MessageFactory::DocumentError("The requested document does not exist (invalid URI)");

		// create a copy of the User object before it gets modified, for rollback support
		User* user = client->getUser();
		User backupUser = *(user);

		qDebug() << "> User" << user->getUsername() << "requested document" << docUri.toString();

		/* check if it's the first time opening this document */
		if (!user->hasDocument(docUri))
		{
			/* add this document to those owned by the user */
			user->addDocument(docUri);

			/* and add the new editor to the document's list of editors */
			documents.find(docUri).value()->insertNewEditor(user->getUsername());

			try {
				db.addDocToUser(user->getUsername(), docUri.toString());
			}
			catch (DatabaseException & dbe) {
				qDebug().noquote() << ">" << "(DB ERROR) Cannot insert: '" << user->getUsername() << " - " << docUri.toString();
				client->getUser()->rollback(backupUser);
				return MessageFactory::DocumentError("Couldn't add the document to your account, please try again");
			}
		}
	}
	
	try
	{
		QSharedPointer<Document> doc = documents.find(docUri).value();

		ws = workspaces.contains(docUri) ?
			workspaces.find(docUri).value() :
			createWorkspace(doc);
	}
	catch (DocumentException& de)
	{
		qDebug().noquote() << ">" << de.what();
		return MessageFactory::DocumentError("Document loading failed, please try again later");
	}

	/* this thread will not recives more messages from client */
	disconnect(clientSocket, &QSslSocket::readyRead, this, &TcpServer::readMessage);
	disconnect(clientSocket, &QSslSocket::disconnected, this, &TcpServer::clientDisconnection);
	disconnect(clientSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &TcpServer::sslSocketError);

	/* move the socket's affinity to the workspace thread */
	QSslSocket* s = client->getSocket();
	s->setParent(nullptr);
	s->moveToThread(ws->thread());

	clients.remove(clientSocket);		// remove the Client from the server map

	/* move the client object from the server to the workspace thread */
	connect(this, &TcpServer::clientToWorkspace, ws.get(), &WorkSpace::newClient);
	emit clientToWorkspace(client);
	disconnect(this, &TcpServer::clientToWorkspace, ws.get(), &WorkSpace::newClient);

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

	// create a copy of the User object before it gets modified, for rollback support
	User* user = client->getUser();
	User backupUser = *(user);

	qDebug() << "> Removing document" << docUri.toString() << "from user" << user->getUsername();

	if (user->hasDocument(docUri))
	{
		/* remove this document to those owned by the user */
		user->removeDocument(docUri);

		try {
			db.removeDocFromUser(user->getUsername(), docUri.toString());
		}
		catch (DatabaseException & dbe) {
			qDebug().noquote() << ">" << "(DB ERROR) Cannot remove: '" << docUri.toString() << "'";
			client->getUser()->rollback(backupUser);
			return MessageFactory::DocumentError("Couldn't remove the document from your account, please try again");
		}
	}
	else 
		return MessageFactory::DocumentError("You don't have access to that document");

	try 
	{
		if (!db.countDocEditors(docUri.toString())) {
			// no one has access to this document --> must be erased
			documents.find(docUri).value()->remove();
			documents.remove(docUri);

			qDebug() << "> Permanently deleted document" << docUri.toString() << "file from disk";
		}
	}
	catch (DatabaseException & de)
	{
		qDebug().noquote() << ">" << de.what();
	}
	

	return MessageFactory::DocumentDismissed();
}

/* Release all resources owned by workspace and delete it */
void TcpServer::deleteWorkspace(URI document)
{
	try /* delete workspace and remove it from the map */
	{
		workspaces.remove(document);

		qDebug().nospace().noquote() << "> Workspace (" << document.toString() << ") successfully closed";
	}
	catch (DocumentException& de)
	{
		qDebug().noquote() << ">" << de.what();
	}
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

	// Read all the available message data from the socket
	dataBuffer = socket->read((qint64)(socketBuffer.getDataSize() - socketBuffer.getReadDataSize()));

	socketBuffer.append(dataBuffer);

	if (socketBuffer.bufferReadyRead()) {
		
		QDataStream dataStream(&(socketBuffer.buffer), QIODevice::ReadWrite);
		quint16 mType = socketBuffer.getType();
		
		try {
			MessageCapsule message = MessageFactory::Empty((MessageType)mType);
			message->read(dataStream);
			socketBuffer.clear();

			if (mType == LoginRequest || mType == LoginUnlock || mType == AccountCreate || mType == AccountUpdate ||
				mType == Logout || mType == DocumentCreate || mType == DocumentOpen || mType == DocumentRemove)
			{
				messageHandler.process(message, socket);
			}
			else
			{
				qDebug() << "> (ERROR) Received unexpected message of type: " << mType;
				message = MessageFactory::Failure(QString("Unknown message type : ") + QString::number(mType));
				message->send(socket);
			}
		}
		catch (MessageTypeException& mte) {
			MessageCapsule message = MessageFactory::Failure(QString("Unknown message type : ") + QString::number(mType));
			message->send(socket);
			qDebug().noquote() << ">" << mte.what();
			socketBuffer.clear();
		}
		catch (MessageException& mre) {
			qDebug().noquote() << ">" << mre.what();
			socketBuffer.clear();
			return;
		}
	}
}


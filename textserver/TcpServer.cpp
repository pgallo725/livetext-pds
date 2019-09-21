#include "TcpServer.h"

#include <iostream>

#include <QFile>
#include <QFileInfo>
#include <QDataStream>
#include <QtNetwork>
#include <QHostAddress>
#include <QDateTime>
#include <QRandomGenerator>
#include <QMutexLocker>

#include <MessageFactory.h>
#include "ServerException.h"

#define INDEX_FILENAME "./Documents/documents.dat"
#define USERS_FILENAME "users.dat"
#define TMP_USERS_FILENAME "users.tmp"


/* Server costructor */
TcpServer::TcpServer(QObject* parent)
	: QTcpServer(parent), messageHandler(this), _userIdCounter(0)
{
	qRegisterMetaType<QSharedPointer<Client>>("QSharedPointer<Client>");
	qRegisterMetaType<URI>("URI");

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
	time.stop();
	saveUsers();

	qDebug() << "Server down";
}

/* Load users and documents */
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
	}
	else
	{
		QFileInfo info(usersFile);
		throw FileLoadException(info.absoluteFilePath().toStdString());
	}

	// Initialize the counter to assign user IDs
	_userIdCounter = users.size();

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
				// THROW: handle error or FileFormatException ?
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
	time.start(USERS_SAVE_TIMEOUT);

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
	QFile file(TMP_USERS_FILENAME);
	if (file.open(QIODevice::WriteOnly))
	{
		QDataStream usersDb(&file);

		/* Serialization of users database must be done in mutual exclusion with
		any other AccountUpdate operations that workspace threads might be performing */
		QMutexLocker locker(&users_mutex);		

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
		else file.rename(USERS_FILENAME);

		file.close();

		std::cout << "done" << std::endl;
	}
	else
	{
		QFileInfo info(file);
		throw FileWriteException(info.absolutePath().toStdString(), info.fileName().toStdString());
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

	return MessageFactory::AccountConfirmed(user);
}


/* Update user's fields and return response message for the client */
MessageCapsule TcpServer::updateAccount(QSslSocket* clientSocket, QString nickname, QImage icon, QString password)
{
	Client* client = clients.find(clientSocket).value().get();

	if (!client->isLogged())
		return MessageFactory::AccountError("You are not logged in");

	User* user = client->getUser();

	if (!nickname.isNull())
		user->setNickname(nickname);
	if (!icon.isNull())
		user->setIcon(icon);
	if (!password.isNull())
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

void TcpServer::restoreUserAvaiable(QString username)
{
	usersNotAvaiable.removeOne(username);
}



/****************************** DOCUMENT METHODS ******************************/


void addToIndex(QSharedPointer<Document> doc)
{
	QFile file(INDEX_FILENAME);
	if (file.open(QIODevice::Append | QIODevice::Text))
	{
		QTextStream indexFileStream(&file);

		indexFileStream << doc->getURI().toString() << endl;

		if (indexFileStream.status() == QTextStream::Status::WriteFailed)
		{
			// THROW: handle error or FileWriteException ?
		}

		file.close();
	}
	else
	{
		QFileInfo info(file);
		throw FileWriteException(info.absolutePath().toStdString(), info.fileName().toStdString());
	}
}


/* Create a new worskpace for a document */
QSharedPointer<WorkSpace> TcpServer::createWorkspace(QSharedPointer<Document> document, QSharedPointer<Client> client)
{
	QSharedPointer<WorkSpace> w = QSharedPointer<WorkSpace>(new WorkSpace(document, users_mutex));
	documents.insert(document->getURI(), document);
	workspaces.insert(document->getURI(), w);
	
	/* workspace will notify when clients quit editing the document and when it becomes empty */
	connect(w.get(), &WorkSpace::returnClient, this, &TcpServer::receiveClient);
	connect(w.get(), &WorkSpace::noEditors, this, &TcpServer::deleteWorkspace);
	connect(w.get(), &WorkSpace::restoreUserAvaiable, this, &TcpServer::restoreUserAvaiable, Qt::QueuedConnection);

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

	QSharedPointer<Document> doc(new Document(docURI));

	/* add the document to the index and save the file */
	addToIndex(doc);
	doc->save();

	QSharedPointer<WorkSpace> w = createWorkspace(doc, client);
	
	/* add to newly created document to those owned by the user */
	client->getUser()->addDocument(docURI);
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

	if (!client->isLogged())
		return MessageFactory::DocumentError("You are not logged in");

	/* check if this document doesn't exist */
	if (!documents.contains(docUri))
		return MessageFactory::DocumentError("The requested document does not exist (invalid URI)");

	QSharedPointer<Document> doc = documents.find(docUri).value();

	QSharedPointer<WorkSpace> w = workspaces.contains(docUri) ?
		workspaces.find(docUri).value() :
		createWorkspace(doc, client);

	User* user = client->getUser();

	/* if it's the first time opening this document, add it to the user's list of documents */	
	if (!user->hasDocument(docUri))
		user->addDocument(docUri);
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
	WorkSpace* w = workspaces.find(document).value().get();

	/* delete workspace and remove it from the map */
	workspaces.remove(document);	
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
			message = MessageFactory::Failure("Unknown message type : " + mType);
			message->sendTo(socket);
		}
	}
}


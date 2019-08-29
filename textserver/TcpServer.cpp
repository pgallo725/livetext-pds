#include "TcpServer.h"

#include <iostream>

#include <QFile>
#include <QDataStream>
#include <QFileInfo>
#include <QtNetwork>
#include <QHostAddress>
#include <QImage>
#include <QDateTime>
#include <QCryptographicHash>

#include <MessageFactory.h>
#include "ServerException.h"


/* Costructor */
TcpServer::TcpServer(QObject* parent)
	: QTcpServer(parent), messageHandler(this), _userIdCounter(0)
{
	qRegisterMetaType<QSharedPointer<Client>>("QSharedPointer<Client>");

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
	time.start(SAVE_TIMEOUT);

	std::cout << "\nServer up" << std::endl;
}

/* Generate the URI for a document */
URI TcpServer::generateURI(QString authorName, QString docName) const
{
	QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
	QString str = authorName + "_" + docName + "_";

	for (int i = 0; i < 12; ++i)	// add a 12-character long random sequence to the document URI to make it unique
	{
		int index = qrand() % possibleCharacters.length();
		QChar nextChar = possibleCharacters.at(index);
		str.append(nextChar);
	}

	return URI(str);
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
	if (socket == nullptr) {
		qDebug() << "ERROR: no pending connections!\n";
		throw SocketNullException("::newClientConnection - no pending connections");		// TODO: could be ignored
	}

	qDebug() << " - new connection from a client";

	QSharedPointer<Client> client(new Client(socket->socketDescriptor()));
	clients.insert(socket, client);

	/* connect slots to be able to read messages */
	connect(socket, &QTcpSocket::readyRead, this, &TcpServer::readMessage);
	connect(socket, &QTcpSocket::disconnected, this, &TcpServer::clientDisconnection);
}

/* handle client disconnection and release resources */
void TcpServer::clientDisconnection()
{
	QTcpSocket* socket = static_cast<QTcpSocket*>(sender());

	qDebug() << " - client disconnected";

	clients.remove(socket);					/* remove this client from the map */
	socket->close();						/* close and destroy the socket */
	socket->deleteLater();
}


/****************************** ACCOUNT METHODS ******************************/


/* create a new client and send nonce to be solved for authentication */
MessageCapsule TcpServer::serveLoginRequest(QTcpSocket* clientSocket, QString username)
{
	qDebug() << "si e' collegato: " << username;

	QSharedPointer<Client> client = clients.find(clientSocket).value();

 	if (users.contains(username))
	{
		if (client->isLogged())
			return MessageFactory::LoginError("Your client is already logged in as '" + client->getUsername() + "'");

		return MessageFactory::LoginChallenge(client->challenge(&(users.find(username).value())));
	}
	else return MessageFactory::LoginError("The specified username is not registered on the server");
}

/* Authenticate the client's User and apply the login */
MessageCapsule TcpServer::authenticateUser(QTcpSocket* clientSocket, QString token)
{
	QSharedPointer<Client> client = clients.find(clientSocket).value();

	if (client->isLogged())
		return MessageFactory::LoginError("You are already logged in");

	if (client->authentication(token))
	{
		client->login(client->getUser());
		return MessageFactory::LoginGranted(*client->getUser());
	}
	else
	{
		client->logout();
		return MessageFactory::LoginError("Wrong username/password");
	}
}

/* Create a new User */
MessageCapsule TcpServer::createAccount(QTcpSocket* socket, User& newUser)
{
	QSharedPointer<Client> client = clients.find(socket).value();
	if (client->isLogged())
		return MessageFactory::AccountError("You cannot create an account while being logged in as another user");

	/* check if username or password are nulls */
	if (!newUser.getUsername().compare("") || !newUser.getPassword().compare(""))
		return MessageFactory::AccountError("Username and/or password must be field");

	/* check if this username is already used */
	if (users.contains(newUser.getUsername()))
		return MessageFactory::AccountError("That username is already taken");
	
	User user(newUser.getUsername(), _userIdCounter++,
		newUser.getNickname(), newUser.getPassword(), newUser.getIcon());			/* create a new user		*/
	QMap<QString, User>::iterator i = users.insert(newUser.getUsername(), user);	/* insert new user in map	*/

	client->login(&(*i));		// client is automatically logged

	return MessageFactory::AccountConfirmed(user.getUserId());
}


/* Update user's fields and return response message for the client */
MessageCapsule TcpServer::updateAccount(QTcpSocket* clientSocket, User& updatedUser)
{
	Client* client = clients.find(clientSocket).value().get();

	if (!client->isLogged())
		return MessageFactory::AccountError("You are not logged in");

	User* oldUser = client->getUser();

	if (oldUser->getUserId() == updatedUser.getUserId() &&
		oldUser->getUsername() == updatedUser.getUsername())
	{
		oldUser->setNickname(updatedUser.getNickname());
		oldUser->setIcon(updatedUser.getIcon());
		oldUser->changePassword(updatedUser.getPassword());

		return MessageFactory::AccountConfirmed(oldUser->getUserId());
	}
	else return MessageFactory::AccountError("Cannot modify a different user's account");
}


/* Release resources owned by a client and delete it */
void TcpServer::logoutClient(QTcpSocket* clientSocket)
{
	clients.remove(clientSocket);					// remove this client from the map 
	clientSocket->close();						// close and destroy the socket 
}

/* Move a client from the workspace that he has exited back to the server */
void TcpServer::receiveClient(QSharedPointer<Client> client)
{
	QTcpSocket* socket = new QTcpSocket;

	if (!socket->setSocketDescriptor(client->getSocketDescriptor())) {
		qDebug() << socket->error();
		return;
	}

	clients.insert(socket, client);

	/* reconnect socket signals to slots in order to read and handle messages */
	connect(socket, &QTcpSocket::readyRead, this, &TcpServer::readMessage);
	connect(socket, &QTcpSocket::disconnected, this, &TcpServer::clientDisconnection);
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


/* create a new worskpace, a new thread and bind the workspace's affinity the the thread*/
WorkSpace* TcpServer::createWorkspace(QSharedPointer<Document> document, QSharedPointer<Client> client)
{
	WorkSpace* w = new WorkSpace(document, QSharedPointer<TcpServer>(this));
	//QSharedPointer<WorkSpace> w = QSharedPointer<WorkSpace>(new WorkSpace(document, QSharedPointer<TcpServer>(this)));
	//QThread* t = new QThread();

	documents.insert(document->getURI(), document);
	workspaces.insert(document->getURI(), QSharedPointer<WorkSpace>(w));
	//workThreads.insert(document->getURI(), QSharedPointer<QThread>(t));

	/* change affinity of this workspace with a new thread */
	//w->moveToThread(t);
	//t->start();

	/* workspace will notify when clients quit editing the document and when it becomes empty */
	connect(w, &WorkSpace::returnClient, this, &TcpServer::receiveClient);
	connect(w, &WorkSpace::noEditors, this, &TcpServer::deleteWorkspace);

	return w;
}

/* Create a new Document */
MessageCapsule TcpServer::createDocument(QTcpSocket* author, QString docName)
{
	QSharedPointer<Client> client = clients.find(author).value();

	if (!client->isLogged())
		return MessageFactory::DocumentError("You are not logged in");

	URI docURI = generateURI(client->getUsername(), docName);

	/* check if documents is already used */
	if (documents.contains(docURI))
		return MessageFactory::DocumentError("A document with the same URI already exists");

	QSharedPointer<Document> doc(new Document(docURI));
	WorkSpace* w = createWorkspace(doc, client);
	QFile docFile(docURI.toString());
	
	/* add the document to the index, create the document file and update internal data structures */
	addToIndex(doc);
	docFile.open(QIODevice::NewOnly);
	docFile.close();
	
	/* add to client list the newDocument */
	client->getUser()->addDocument(docURI);
	/* add to document list the new editor */
	doc->insertNewEditor(client->getUsername());

	/* this thread will not receives more messages from client */
	/*disconnect(author, &QTcpSocket::readyRead, this, &TcpServer::readMessage);	
	disconnect(author, &QTcpSocket::disconnected, this, &TcpServer::clientDisconnection);*/

	/* make the new thead connect the socket in the workspace */
	/*connect(this, &TcpServer::newSocket, w, &WorkSpace::newSocket);		
	emit newSocket(static_cast<qint64>(author->socketDescriptor()));	
	disconnect(this, &TcpServer::newSocket, w, &WorkSpace::newSocket);*/

	clients.remove(author);		// remove the Client from the server map

	connect(this, &TcpServer::clientToWorkspace, w, &WorkSpace::newClient);
	emit clientToWorkspace(std::move(client));
	disconnect(this, &TcpServer::clientToWorkspace, w, &WorkSpace::newClient);

	//author->deleteLater();		// TODO: delete socket ??

	return MessageCapsule();
}

/* Open an existing Document */
MessageCapsule TcpServer::openDocument(QTcpSocket* clientSocket, URI docUri)
{
	QSharedPointer<Client> client = clients.find(clientSocket).value();

	if (!client->isLogged())
		return MessageFactory::DocumentError("You are not logged in");

	/* check if this document doesn't exist */
	if (!documents.contains(docUri))
		return MessageFactory::DocumentError("The requested document does not exist (invalid URI)");

	QSharedPointer<Document> doc = documents.find(docUri).value();
	WorkSpace* w = nullptr;

	/* check if this documents is already opened in a Workspace or not */
	if (!workspaces.contains(docUri)) {
		w = createWorkspace(doc, client);
	}
	else {
		w = workspaces.find(docUri).value().get();
	}

	/* add to client list the newDocument*/
	client->getUser()->addDocument(docUri);
	/* add to document list the new editor */
	doc->insertNewEditor(client->getUsername());

	/* this thread will not recives more messages from client */
	/*disconnect(clientSocket, &QTcpSocket::readyRead, this, &TcpServer::readMessage);
	disconnect(clientSocket, &QTcpSocket::disconnected, this, &TcpServer::clientDisconnection);*/

	/* make the new thead connect the socket in the workspace */
	/*connect(this, &TcpServer::newSocket, w, &WorkSpace::newSocket);
	emit newSocket(static_cast<qint64>(author->socketDescriptor()));
	disconnect(this, &TcpServer::newSocket, w, &WorkSpace::newSocket);*/

	clients.remove(clientSocket);		// remove the Client from the server map

	connect(this, &TcpServer::clientToWorkspace, w, &WorkSpace::newClient);
	emit clientToWorkspace(client);
	disconnect(this, &TcpServer::clientToWorkspace, w, &WorkSpace::newClient);

	//clientSocket.deleteLater();		// TODO: delete socket ??

	return MessageCapsule();
}

/* Delete a document from the client's list */
MessageCapsule TcpServer::removeDocument(QTcpSocket* clientSocket, URI docUri)
{
	QSharedPointer<Client> client = clients.find(clientSocket).value();

	if (!client->isLogged())
		return MessageFactory::DocumentError("You are not logged in");

	if (!documents.contains(docUri))
		return MessageFactory::DocumentError("The specified document does not exist (invalid URI)");

	client->getUser()->removeDocument(docUri);
	return MessageFactory::DocumentDismissed();
}


/* release all resource owned by workspace, delete it and its thread */
void TcpServer::deleteWorkspace(URI document)
{
	WorkSpace* w = workspaces.find(document).value().get();
	//QThread* t = workThreads.find(document).value().get();

	/* they throw an exception ??? why??? */
	//disconnect(w, &WorkSpace::noEditors, this, &TcpServer::deleteWorkspace);
	//disconnect(w, &WorkSpace::deleteClient, this, &TcpServer::deleteClient);

	/* delete workspace and remove it from the map */
	workspaces.remove(document);	

	/* stop the thread, wait for exit, delete thread and remove it from the map */
	//workThreads.find(document).value()->quit();
	//workThreads.find(document).value()->wait();
	//workThreads.remove(document);

	qDebug() << "workspace (" << document.toString() << ") closed";
}


/****************************** MESSAGES METHODS ******************************/


/* read the message from the socket and create the correct type message for the server */
void TcpServer::readMessage()
{
	QTcpSocket* socket = static_cast<QTcpSocket*>(sender());

	QDataStream streamIn(socket);	/* connect stream with socket */

	quint16 mType;
	streamIn >> mType;		 /* take the type of incoming message */

	MessageCapsule message = MessageFactory::Empty((MessageType)mType);
	message->readFrom(streamIn);

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



/****************************** SERVER METHODS ******************************/


/* allow to a secondary thread to steal a QSharedPointer from the server */
/*QSharedPointer<Client> TcpServer::moveClient(qintptr socketDescriptor)
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
}*/

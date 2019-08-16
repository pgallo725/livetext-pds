#include "TcpServer.h"

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
	: QTcpServer(parent), messageHandler(this), _userIdCounter(0)
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
			//createAccount("admin", "sudo", "admin", QImage(), nullptr);		// TODO: need to remove this at all costs
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

	qDebug() << " - client '" << clients.find(socket).value()->getUsername() << "' disconnected";

	clients.remove(socket);					/* remove this client from the map */
	socket->close();						/* close the socket */
}


/****************************** ACCOUNT METHODS ******************************/


/* create a new client and send nonce to be solved for authentication */
MessageCapsule TcpServer::serveLoginRequest(QTcpSocket* clientSocket, QString username)
{
	qDebug() << " -- (socket " << clientSocket << ") si e' collegato: " << username;

	QSharedPointer<Client> client = clients.find(clientSocket).value();

	if (users.contains(username))
	{
		if (client->isLogged())
			return new LoginMessage(LoginError, "Your client is already logged in as '" + client->getUsername() + "'");

		return new LoginMessage(LoginChallenge, client->challenge(&(users.find(username).value())));
	}
	else return new LoginMessage(LoginError, "The specified username is not registered on the server");
}

/* Authenticate the client's User and apply the login */
MessageCapsule TcpServer::authenticateUser(QTcpSocket* clientSocket, QString token)
{
	QSharedPointer<Client> client = clients.find(clientSocket).value();

	if (client->isLogged())
		return new LoginMessage(LoginError, "You are already logged in");

	if (client->authentication(token))
	{
		client->login(client->getUser());
		return new LoginMessage(LoginAccessGranted, *client->getUser());
	}
	else
	{
		client->logout();
		return new LoginMessage(LoginError, "Wrong username/password");
	}
}

/* Create a new User */
MessageCapsule TcpServer::createAccount(QTcpSocket* socket, User& newUser)
{
	QSharedPointer<Client> client = clients.find(socket).value();
	if (client->isLogged())
		return new AccountMessage(AccountDenied, "You cannot create an account while being logged in as another user");

	/* check if this username is already used */
	if (users.contains(newUser.getUsername()))
		return new AccountMessage(AccountDenied, "That username is already taken");
	
	User user(newUser.getUsername(), _userIdCounter++,
		newUser.getNickname(), newUser.getPassword(), newUser.getIcon());			/* create a new user		*/
	QMap<QString, User>::iterator i = users.insert(newUser.getUsername(), user);	/* insert new user in map	*/

	client->login(&(*i));		// client is automatically logged

	return new AccountMessage(AccountConfirmed, user.getUserId());
}


/* Update user's fields and return response message for the client */
MessageCapsule TcpServer::updateAccount(QTcpSocket* clientSocket, User& updatedUser)
{
	Client* client = clients.find(clientSocket).value().get();

	if (!client->isLogged())
		return new AccountMessage(AccountDenied, "You are not logged in");

	User* oldUser = client->getUser();

	if (oldUser->getUserId() == updatedUser.getUserId() &&
		oldUser->getUsername() == updatedUser.getUsername())
	{
		oldUser->setNickname(updatedUser.getNickname());
		oldUser->setIcon(updatedUser.getIcon());
		oldUser->changePassword(updatedUser.getPassword());

		return new AccountMessage(AccountConfirmed);
	}
	else return new AccountMessage(AccountDenied, "Cannot modify a different user's account");
}


/* release resources own by a client and delete it */
MessageCapsule TcpServer::logoutClient(QTcpSocket* clientSocket)
{
	Client* client = clients.find(clientSocket).value().get();

	if (!client->isLogged())
		return new LogoutMessage(LogoutDenied, "You cannot logout if you're not logged in");

	client->logout();

	return new LogoutMessage(LogoutConfirmed);
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
WorkSpace* TcpServer::createWorkspace(QSharedPointer<Document> document, QSharedPointer<Client> client)
{
	WorkSpace* w = new WorkSpace(document, QSharedPointer<TcpServer>(this));
	//QSharedPointer<WorkSpace> w = QSharedPointer<WorkSpace>(new WorkSpace(document, QSharedPointer<TcpServer>(this)));
	QThread* t = new QThread();

	documents.insert(document->getURI(), document);
	workspaces.insert(document->getURI(), QSharedPointer<WorkSpace>(w));
	workThreads.insert(document->getURI(), QSharedPointer<QThread>(t));

	/* change affinity of this workspace with a new thread */
	w->moveToThread(t);
	t->start();

	/* workspace will notify when theres no one using it for delete */
	connect(w, &WorkSpace::notWorking, this, &TcpServer::deleteWorkspace);

	return w;
}

/* Create a new Document */
MessageCapsule TcpServer::createDocument(QTcpSocket* author, QString docName)
{
	QSharedPointer<Client> client = clients.find(author).value();

	if (!client->isLogged())
		return new DocumentMessage(DocumentError, "You are not logged in");

	QString docURI = client->getUsername() + "_" + docName + "_" + "deadbeef";		/* TODO: random sequence */

	/* check if documents is already used */
	if (documents.contains(docURI))
		return new DocumentMessage(DocumentError, "A document with the same URI already exists");

	QSharedPointer<Document> doc(new Document(docURI));
	WorkSpace* w = createWorkspace(doc, client);
	QFile docFile(docURI);
	
	/* add the document to the index, create the document file and update internal data structures */
	addToIndex(doc);
	docFile.open(QIODevice::NewOnly);
	docFile.close();
	
	/* add to client list the newDocument */
	client->getUser()->addDocument(docURI);
	/* add to document list the new editor */
	doc->insertNewEditor(client->getUsername());

	/* this thread will not receives more messages from client */
	disconnect(author, &QTcpSocket::readyRead, this, &TcpServer::readMessage);	
	disconnect(author, &QTcpSocket::disconnected, this, &TcpServer::clientDisconnection);

	/* make the new thead connect the socket in the workspace */
	connect(this, &TcpServer::newSocket, w, &WorkSpace::newSocket);		
	emit newSocket(static_cast<qint64>(author->socketDescriptor()));	
	disconnect(this, &TcpServer::newSocket, w, &WorkSpace::newSocket);

	return new DocumentMessage(DocumentOpened, *doc);
}

/* Open an existing Document */
MessageCapsule TcpServer::openDocument(QTcpSocket* clientSocket, QString docUri)
{
	QSharedPointer<Client> client = clients.find(clientSocket).value();

	if (!client->isLogged())
		return new DocumentMessage(DocumentError, "You are not logged in");

	/* check if this document doesn't exist */
	if (!documents.contains(docUri))
		return new DocumentMessage(DocumentError, "The requested document does not exist (invalid URI)");

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
	disconnect(clientSocket, &QTcpSocket::readyRead, this, &TcpServer::readMessage);
	disconnect(clientSocket, &QTcpSocket::disconnected, this, &TcpServer::clientDisconnection);

	/* make the new thead connect the socket in the workspace */
	connect(this, &TcpServer::newSocket, w, &WorkSpace::newSocket);
	emit newSocket(static_cast<qint64>(clientSocket->socketDescriptor()));
	disconnect(this, &TcpServer::newSocket, w, &WorkSpace::newSocket);

	return new DocumentMessage(DocumentOpened, *doc);
}

/* Delete a document from the client's list */
MessageCapsule TcpServer::removeDocument(QTcpSocket* clientSocket, QString docUri)
{
	QSharedPointer<Client> client = clients.find(clientSocket).value();

	if (!client->isLogged())
		return new DocumentMessage(DocumentError, "You are not logged in");

	if (!documents.contains(docUri))
		return new DocumentMessage(DocumentError, "The specified document does not exist (invalid URI)");

	client->getUser()->removeDocument(docUri);
	return new DocumentMessage(DocumentRemoved);
}


/* release all resource owned by workspace, delete it and its thread */
void TcpServer::deleteWorkspace(QString document)
{
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
	MessageCapsule message;

	streamIn.setDevice(socket); /* connect stream with socket */

	streamIn >> typeOfMessage;	/* take the type of incoming message */

	try {
		switch (typeOfMessage)
		{
			/* LoginMessages */

		case LoginRequest:
		case LoginUnlock:
			message = new LoginMessage((MessageType)typeOfMessage);
			message->readFrom(streamIn);
			break;
	
			/* AccountMessages */

		case AccountCreate:
		case AccountUpdate:
			message = new AccountMessage((MessageType)typeOfMessage);
			message->readFrom(streamIn);
			break;

			/* LogoutMessages */

		case LogoutRequest:
			message = new LogoutMessage((MessageType)typeOfMessage);
			message->readFrom(streamIn);
			break;

			/* DocumentMessages */

		case NewDocument:
		case OpenDocument:
		case RemoveDocument:
			message = new DocumentMessage((MessageType)typeOfMessage);
			message->readFrom(streamIn);
			break;

		default:
			throw MessageUnexpectedTypeException(message->getType());
			return;
		}

		messageHandler.process(message, socket);
	}
	catch (MessageUnexpectedTypeException& e) {
		message = new ErrorMessage(MessageTypeError, e.what());
		message->sendTo(socket);
	}
	catch (UserNotFoundException& e) {
		
	}
}



/****************************** SERVER METHODS ******************************/


/* allow to a secondary thread to steal a QSharedPointer from the server */
QSharedPointer<Client> TcpServer::moveClient(qintptr socketDescriptor)
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

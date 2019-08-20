#include "Client.h"

Client::Client(QObject* parent) : QObject(parent)
{
	socket = new QTcpSocket(this);

	connect(socket, SIGNAL(connected()), this, SLOT(serverConnection()));
	connect(socket, SIGNAL(disconnected()), this, SLOT(serverDisconnection()));
	connect(socket, SIGNAL(error(QAbstractSocket::SocketError socketError)), this, SLOT(errorHandle()));

	connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
		[&](QAbstractSocket::SocketError socketError) { // Error in connection
	emit impossibleToConnect();
	socket->abort(); });

	//socket->connectToHost("127.0.0.1", 1500);
}


Client::~Client()
{
	// TODO
}

void Client::writeOnServer()
{

}

void Client::serverConnection() {
	qDebug() << "Client connesso al server ";
	emit connectionEstablished();
}

void Client::serverDisconnection() {
	qDebug() << "Client disconnesso dal server ";
}

void Client::errorHandler() {

	qDebug() << "error raised";
	socket->close();

}

void Client::readBuffer() {

	qDebug() << "reading socket";
	
	quint16 typeOfMessage;
	QDataStream in;

	in >> typeOfMessage;

	switch (typeOfMessage) {
	case MoveCursor:
		reciveCursor(in);
		break;
	case UserAccountUpdate:
		accountUpdate(in);
		break;
	case AddUserPresence:
		newUserPresence(in);
			break;
	default:
		//thorw exception
		break;
	}
}

void Client::Connect(QString ipAddress, quint16 port) {
	socket->connectToHost(ipAddress, port);
	return;
}

void Client::Disconnect() {
	socket->close();
	qDebug() << "connection closed by client";
}

void Client::setUsername(QString username) {

	this->username = username;

}

void Client::setPassword(QString password) {

	this->password = password;

}

void Client::setNickname(QString nickname) {

	this->nickname = nickname;
}

void Client::setImage(QImage image) {
	this->image = image;
}

void Client::setLogin(bool flag) {
	login = flag;
}

bool Client::getLogin() {
	return login;
}

void Client::Login() {

	QString error;
	MessageCapsule loginRequest = new LoginMessage(LoginRequest,username);

	loginRequest->sendTo(socket);

	if (!socket->waitForReadyRead(10000)) {
		qDebug() << "recived no byte";
		//throw ServerNotRespondException();
		error = "recived no byte";
		emit loginFailed(error);
		return;
	}

	QDataStream in;
	qint16 typeOfMessage;

	in.setDevice(socket);
	in >> typeOfMessage;

	// switch to check the correctness of the type of Message
	switch (typeOfMessage) {
	case LoginChallenge:
		break; 
	case LoginError:
		// user not exist
		MessageCapsule loginError = new LoginMessage(LoginError);
		loginError->readFrom(in);
		LoginMessage* loginerror = dynamic_cast<LoginMessage*>(loginError.get());
		emit loginFailed(loginerror->getErrorMessage());
		return;
		break;
	default:
		//throw MessageUnknownTypeException();
		//EMIT ?
		return;
		break;
	}

	MessageCapsule loginChallenge = new LoginMessage(LoginChallenge);

	loginChallenge->readFrom(in);

	LoginMessage *docMsg = dynamic_cast<LoginMessage*>(loginChallenge.get());
	QString nonce = docMsg->getNonce();

	qDebug() << "cripting salt " << nonce;
	QString result = password + nonce;
	/*hash.addData(password.toStdString().c_str(), password.length());

	out << typeOfMessage << QString::fromStdString(hash.result().toStdString());
	*/
	LoginMessage* loginUnlock = new LoginMessage(LoginUnlock, result);
	loginUnlock->sendTo(socket);

	if (!socket->waitForReadyRead(10000)) {
		qDebug() << "recived no byte";
		//throw ServerNotRespondException();
		error = "recived no byte";
		emit loginFailed(error);
		return;
	}

	in >> typeOfMessage;

	switch (typeOfMessage) {
	case LoginAccessGranted:
		LoginMessage* loginAccess = new LoginMessage(LoginAccessGranted);
		loginAccess->readFrom(in);
		emit loginSuccess();
		return;

		break;
	case LoginError:
		// user not exist
		MessageCapsule loginError = new LoginMessage(LoginError);
		loginError->readFrom(in);
		LoginMessage* loginerror = dynamic_cast<LoginMessage*>(loginError.get());
		emit loginFailed(loginerror->getErrorMessage());
		return;
		break;
	default:
		//throw MessageUnknownTypeException();
		//EMIT ?
		return;
		break;
	}
}

void Client::Register() {

	quint16 typeOfMessage;
	QDataStream in(socket);
	QString error;
	// Link the stream to the socke and send the byte

	User* user = new User(username,-1,nickname,password,image);

	MessageCapsule accountCreate = new AccountMessage(AccountCreate, *user);

	accountCreate->sendTo(socket);

	//wait the response from the server
	if (!socket->waitForReadyRead(10000)) {
		qDebug() << "recived no byte";
		//throw ServerNotRespondException();
		error = "recived no byte";
		emit registrationFailed(error);
		return ;
	}

	in >> typeOfMessage;

	switch (typeOfMessage) {
	case AccountConfirmed:
		MessageCapsule accountConfirmed = new AccountMessage(AccountConfirmed);
		accountConfirmed->readFrom(in);
		AccountMessage* accConf = dynamic_cast<AccountMessage *>(accountConfirmed.get());
		user->setId(accConf->getUserId);
		emit registrationCompleted();
		return ;
		break;
	case AccountDenied:
		// impossible to create the account
		MessageCapsule accountDenied = new AccountMessage(AccountDenied);
		accountDenied->readFrom(in);
		AccountMessage* accountdenied = dynamic_cast<AccountMessage*>(accountDenied.get());
		emit registrationFailed(accountdenied->getErrorMessage());
		return ;
		break;
	default:
		//throw MessageUnknownTypeException();
		// EMIT?
		return ;
		break;
	}
}

void Client::Logout() {

	QString error;
	quint16 typeOfMessage;
	QDataStream in(socket);

	MessageCapsule logoutRequest = new LogoutMessage(LogoutRequest);
	logoutRequest->sendTo(socket);

	//wait the response from the server
	if (!socket->waitForReadyRead(10000)) {
		qDebug() << "recived no byte";
		//throw ServerNotRespondException();
		error = "recived no byte";
		emit logoutFailed(error);
		return;
	}

	in >> typeOfMessage;

	switch (typeOfMessage) {
	case LogoutConfirmed:
		MessageCapsule logoutConfirmed = new LogoutMessage(LogoutConfirmed);
		logoutConfirmed->readFrom(in);
		return;
		break;
	case LogoutDenied:
		// impossible to create the account
		MessageCapsule logoutDenied = new AccountMessage(LogoutDenied);
		logoutDenied->readFrom(in);
		AccountMessage* logoutdenied = dynamic_cast<AccountMessage*>(logoutDenied.get());
		emit logoutFailed(logoutdenied->getErrorMessage());
		return;
		break;
	default:
		//throw MessageUnknownTypeException();
		// EMIT?
		return;
		break;
	}

}

void Client::requestURI() {
	
	QString error;
	QDataStream in(socket);
	qint16 typeOfMessage;

	//TODO: add Message to Request URI (function called from LiveText dopo login o register)
}

void Client::openDocument(QString URI) {

	QString error;
	quint16 typeOfMessage = OpenDocument;
	QDataStream in(socket);

	MessageCapsule openDocument = new DocumentMessage(OpenDocument,URI);
	openDocument->sendTo(socket);

	//wait the response from the server
	if (!socket->waitForReadyRead(10000)) {
		error = "recived no byte";
		//throw ServerNotRespondException();
		emit openFileFailed(error);
		return;
	}

	in >> typeOfMessage;

	switch (typeOfMessage) {
	case DocumentOpened:
		// TODO Message
		MessageCapsule documentOpened = new DocumentMessage(DocumentOpened);
		documentOpened->readFrom(in);
		DocumentMessage *documentopened = dynamic_cast<DocumentMessage*>(documentOpened.get());
		emit openFileCompleted(documentopened->getDocument());
		connect(socket, SIGNAL(readyRead()), this, SLOT(readBuffer()));
		return;
		break;
	case DocumentError:
		// impossible to create the account
		MessageCapsule documentError = new DocumentMessage(DocumentError);
		documentError->readFrom(in);
		DocumentMessage* documenterror = dynamic_cast<DocumentMessage*>(documentError.get());
		emit openFileFailed(documenterror->getErrorMessage());
		return;
		break;
	default:
		//throw MessageUnknownTypeException();
		//EMIT ?
		return;
		break;
	}
}

void Client::createDocument(QString name) {

	QString error;
	quint16 typeOfMessage = NewDocument;
	QDataStream in(socket);

	MessageCapsule newDocument = new DocumentMessage(NewDocument, name);
	newDocument->sendTo(socket);

	//wait the response from the server
	if (!socket->waitForReadyRead(10000)) {
		error =  "recived no byte";
		//throw ServerNotRespondException();
		return;
	}

	in >> typeOfMessage;

	switch (typeOfMessage) {
	case DocumentOpened:
		MessageCapsule documentOpened = new DocumentMessage(DocumentOpened);
		documentOpened->readFrom(in);
		DocumentMessage* documentopened = dynamic_cast<DocumentMessage*>(documentOpened.get());
		emit openFileCompleted(documentopened->getDocument());
		connect(socket, SIGNAL(readyRead()), this, SLOT(readBuffer()));
		return;
		break;
	case DocumentError:
		// impossible to create the account
		MessageCapsule documentError = new DocumentMessage(DocumentError);
		documentError->readFrom(in);
		DocumentMessage* documenterror = dynamic_cast<DocumentMessage*>(documentError.get());
		emit openFileFailed(documenterror->getErrorMessage());
		return;
		break;
	default:
		//throw MessageUnknownTypeException();
		//EMIT ?
		return;
		break;
	}

}

void Client::sendCursor(qint32 position) {

	quint16 typeOfMessage = MoveCursor;

	// Link the stream to the socke and send the byte
	MessageCapsule moveCursor = new PresenceMessage(MoveCursor,position);
	moveCursor->sendTo(socket);
	return;
}

void Client::reciveCursor(QDataStream& in) {
	
	MessageCapsule moveCursor = new PresenceMessage(MoveCursor);
	moveCursor->readFrom(in);
	PresenceMessage* movecursor = dynamic_cast<PresenceMessage *>(moveCursor.get());
	emit cursorMoved(movecursor->getCursorPosition(), movecursor->getUserId());

}

void Client::sendChar(Symbol character) {

	//TODO message implementation

}

void Client::reciveChar() {

	//TODO message implementation
}

void Client::accountUpdate(QDataStream& in) {

	MessageCapsule accountUpdate = new PresenceMessage(UserAccountUpdate);
	accountUpdate->readFrom(in);
	PresenceMessage *accountupdate = dynamic_cast<PresenceMessage*>(accountUpdate.get());
	emit accountModified(accountupdate->getUserId(),accountupdate->getNickname(),accountupdate->getIcon());

}

void Client::sendAccountUpdate(qint32 userId,QString name,QImage image) {
	
	MessageCapsule accountUpdate = new PresenceMessage(UserAccountUpdate, userId, name, image);
	accountUpdate->sendTo(socket);

}

void Client::newUserPresence(QDataStream& in) {

	MessageCapsule newAccountPresence = new PresenceMessage(AddUserPresence);
	newAccountPresence->readFrom(in);
	PresenceMessage* newaccountpresence = dynamic_cast<PresenceMessage*>(newAccountPresence.get());
	emit UserPresence(newaccountpresence->getUserId(), newaccountpresence->getNickname(), newaccountpresence->getIcon());

}
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

	messageHandler(typeOfMessage, in);

}

void Client::messageHandler(qint16 typeOfMessage, QDataStream& in) {

	switch (typeOfMessage) {
	case CursorMove:
		reciveCursor(in);
		break;
	case AccountUpdate:
		accountUpdate(in);
		break;
	case PresenceAdd:
		newUserPresence(in);
		break;
	case PresenceRemove:
		deleteUserPresence(in);
		break;
	case CharInsert:
		reciveChar(in);
		break;
	case CharDelete:
		deleteChar(in);
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

	MessageCapsule incomingMessage;
	MessageCapsule loginRequest = MessageFactory::LoginRequest(username);

	loginRequest->sendTo(socket);

	if (!socket->waitForReadyRead(10000)) {
		emit loginFailed("Server not responding");
		return;
	}

	QDataStream in;
	qint16 typeOfMessage;

	in.setDevice(socket);
	in >>  typeOfMessage;

	incomingMessage = MessageFactory::Empty((MessageType) typeOfMessage);
	incomingMessage->readFrom(in);

	// switch to check the correctness of the type of Message
	switch (typeOfMessage) {
	case LoginChallenge:
		break; 
	case LoginError:
	{
		// user not exist
		LoginErrorMessage* loginError = dynamic_cast<LoginErrorMessage*>(incomingMessage.get());
		emit loginFailed(loginError->getErrorMessage());
		return;
	}
	default:
		//throw MessageUnknownTypeException();
		//EMIT ?
		return;
	}

	LoginChallengeMessage *loginChallenge = dynamic_cast<LoginChallengeMessage*>(incomingMessage.get());
	QString nonce = loginChallenge->getNonce();

	qDebug() << "cripting salt " << nonce;
	QString result = password + nonce;
	
	QCryptographicHash hash(QCryptographicHash::Md5);
	hash.addData(result.toStdString().c_str(), result.length());

	MessageCapsule loginUnlock = MessageFactory::LoginUnlock(QString::fromStdString(hash.result().toStdString()));

	loginUnlock->sendTo(socket);

	if (!socket->waitForReadyRead(10000)) {
		emit loginFailed(tr("Server not responding"));
		return;
	}

	in >> typeOfMessage;
	incomingMessage = MessageFactory::Empty((MessageType)typeOfMessage);
	incomingMessage->readFrom(in);

	switch (typeOfMessage) {
	case LoginGranted: {
		LoginGrantedMessage* loginGranted = dynamic_cast<LoginGrantedMessage*>(incomingMessage.get());
		emit loginSuccess(loginGranted->getLoggedUser());
		return;
	}
	case LoginError: {
		// user not exist
		LoginErrorMessage* loginerror = dynamic_cast<LoginErrorMessage*>(incomingMessage.get());
		emit loginFailed(loginerror->getErrorMessage());
		return;
	}
	default:
		//throw MessageUnknownTypeException();
		//EMIT ?
		return;
	}
}

void Client::Register() {

	quint16 typeOfMessage;
	QDataStream in(socket);
	MessageCapsule incomingMessage;
	// Link the stream to the socke and send the byte

	User* user = new User(username,-1,nickname,password,image);

	MessageCapsule accountCreate = MessageFactory::AccountCreate(*user);
	accountCreate->sendTo(socket);

	//wait the response from the server
	if (!socket->waitForReadyRead(10000)) {
		emit registrationFailed(tr("Server not responding"));
		return ;
	}

	in >> typeOfMessage;
	incomingMessage = MessageFactory::Empty((MessageType)typeOfMessage);
	incomingMessage->readFrom(in);

	switch (typeOfMessage) {
	case AccountConfirmed: {
		AccountConfirmedMessage* accountConfirmed = dynamic_cast<AccountConfirmedMessage*>(incomingMessage.get());
		user->setId(accountConfirmed->getUserId());
		emit registrationCompleted(*user);
		return;
	}
	case AccountError: {
		// impossible to create the account
		AccountErrorMessage *accountDenied = dynamic_cast<AccountErrorMessage*>(incomingMessage.get());
		emit registrationFailed(accountDenied->getErrorMessage());
		return;
	}
	default:
		//throw MessageUnknownTypeException();
		// EMIT?
		return ;
	}
}

void Client::Logout() {

	quint16 typeOfMessage;
	QDataStream in(socket);
	MessageCapsule incomingMessage;

	MessageCapsule logoutRequest = MessageFactory::Logout();
	logoutRequest->sendTo(socket);

}

/*--------------------------- DOCUMENT HANDLER --------------------------------*/
void Client::openDocument(QString URI) {

	quint16 typeOfMessage;
	QDataStream in(socket);
	MessageCapsule incomingMessage;

	MessageCapsule openDocument = MessageFactory::DocumentOpen(URI);
	openDocument->sendTo(socket);

	//wait the response from the server
	if (!socket->waitForReadyRead(10000)) {
		emit openFileFailed(tr("Server not responding"));
		return;
	}

	in >> typeOfMessage;
	incomingMessage = MessageFactory::Empty((MessageType)typeOfMessage);
	incomingMessage->readFrom(in);

	switch (typeOfMessage) {
	case DocumentReady: {
		// Open Succeded
		DocumentReadyMessage* documentOpened = dynamic_cast<DocumentReadyMessage*>(incomingMessage.get());
		connect(socket, SIGNAL(readyRead()), this, SLOT(readBuffer()));
		emit openFileCompleted(documentOpened->getDocument());
		return;
	}
	case DocumentError: {
		// impossible to open the Document
		DocumentErrorMessage* documentError = dynamic_cast<DocumentErrorMessage*>(incomingMessage.get());
		emit openFileFailed(documentError->getErrorMessage());
		return;
	}
	default:
		//throw MessageUnknownTypeException();
		//EMIT ?
		return;
	}
}

void Client::createDocument(QString name) {

	quint16 typeOfMessage;
	QDataStream in(socket);
	MessageCapsule incomingMessage;

	MessageCapsule newDocument = MessageFactory::DocumentCreate(name);
	newDocument->sendTo(socket);

	//wait the response from the server
	if (!socket->waitForReadyRead(10000)) {
		emit openFileFailed(tr("Server not responding"));
		return;
	}

	in >> typeOfMessage;
	incomingMessage = MessageFactory::Empty((MessageType)typeOfMessage);
	incomingMessage->readFrom(in);

	switch (typeOfMessage) {
	case DocumentReady: {
		//Document successfully opened
		DocumentReadyMessage* documentOpened = dynamic_cast<DocumentReadyMessage*>(incomingMessage.get());
		connect(socket, SIGNAL(readyRead()), this, SLOT(readBuffer()));
		emit openFileCompleted(documentOpened->getDocument());
		return;
	}
	case DocumentError: {
		// impossible to open the document
		DocumentErrorMessage* documentError = dynamic_cast<DocumentErrorMessage*>(incomingMessage.get());
		emit openFileFailed(documentError->getErrorMessage());
		return;
	}
	default:
		//throw MessageUnknownTypeException();
		//EMIT ?
		return;
	}

}

void Client::deleteDocument(QString URI) {
	
	quint16 typeOfMessage;
	QDataStream in(socket);
	MessageCapsule incomingMessage;

	MessageCapsule removeDocument = MessageFactory::DocumentRemove(URI);
	removeDocument->sendTo(socket);

	if (!socket->waitForReadyRead(10000)) {
		emit removeFileFailed(tr("Server not responding"));
		return;
	}


	in >> typeOfMessage;
	incomingMessage = MessageFactory::Empty((MessageType)typeOfMessage);
	incomingMessage->readFrom(in);

	switch (typeOfMessage) {
	case DocumentDismissed: {
		//Document successfully opened
		emit documentDismissed();
		return;
	}
	case DocumentError: {
		// impossible to open the document
		DocumentErrorMessage* documentError = dynamic_cast<DocumentErrorMessage*>(incomingMessage.get());
		emit removeFileFailed(documentError->getErrorMessage());
		return;
	}
	default:
		//throw MessageUnknownTypeException();
		//EMIT ?
		return;
	}

}

/*--------------------------- CURSOR HANDLER --------------------------------*/

void Client::sendCursor(qint32 userId,qint32 position) {

	quint16 typeOfMessage;

	// Link the stream to the socke and send the byte
	MessageCapsule moveCursor = MessageFactory::CursorMove(userId,position);
	moveCursor->sendTo(socket);
	return;
}

void Client::reciveCursor(QDataStream& in) {
	
	MessageCapsule moveCursor = MessageFactory::Empty(CursorMove);
	moveCursor->readFrom(in);
	CursorMoveMessage* movecursor = dynamic_cast<CursorMoveMessage*>(moveCursor.get());
	emit cursorMoved(movecursor->getCursorPosition(), movecursor->getUserId());

}


/*--------------------------- CHARACTER HANDLER --------------------------------*/
void Client::sendChar(Symbol character) {

	MessageCapsule sendChar = MessageFactory::CharInsert(character);
	sendChar->sendTo(socket);

}

void Client::reciveChar(QDataStream& in) {

	MessageCapsule reciveChar = MessageFactory::Empty(CharInsert);
	reciveChar->readFrom(in);
	CharInsertMessage *recivechar = dynamic_cast<CharInsertMessage*>(reciveChar.get());
	emit recivedSymbol(recivechar->getSymbol());
}

void Client::deleteChar(QDataStream& in) {

	MessageCapsule deleteChar = MessageFactory::Empty(CharDelete);
	deleteChar->readFrom(in);
	CharDeleteMessage* deletechar = dynamic_cast<CharDeleteMessage*>(deleteChar.get());
	emit removeSymbol(deletechar->getPosition());
}


/*--------------------------- ACCOUNT HANDLER --------------------------------*/
void Client::sendAccountUpdate(User userUpdate) {
	
	qint16 typeOfMessage;
	QDataStream in(socket);
	
	disconnect(socket, SIGNAL(readyRead()), this, SLOT(readBuffer()));
	MessageCapsule accountUpdate = MessageFactory::AccountUpdate(userUpdate);
	accountUpdate->sendTo(socket);

	while (true) {

		if (!socket->waitForReadyRead(10000)) {
			emit accountModificationFail(tr("Server not responding"));
			return;
		}

		in >> typeOfMessage;

		switch (typeOfMessage) {
		case AccountConfirmed: {
			MessageCapsule recivedAccountUpdate = MessageFactory::Empty(AccountConfirmed);
			recivedAccountUpdate->readFrom(in);
			AccountUpdateMessage* accountupdate = dynamic_cast<AccountUpdateMessage*>(recivedAccountUpdate.get());
			connect(socket, SIGNAL(readyRead()), this, SLOT(readBuffer()));
			emit personalAccountModified(accountupdate->getUserObj());
			return;
		}
		case AccountError: {
			MessageCapsule accountError = MessageFactory::Empty(AccountError);
			accountError->readFrom(in);
			AccountErrorMessage* accounterror = dynamic_cast<AccountErrorMessage*>(accountError.get());
			connect(socket, SIGNAL(readyRead()), this, SLOT(readBuffer()));
			emit accountModificationFail(accounterror->getErrorMessage());
			return;
		}
			break;
		default:
			messageHandler(typeOfMessage, in);
			break;
		}
	}
}


/*--------------------------- PRESENCE HANDLER --------------------------------*/

void Client::accountUpdate(QDataStream& in) {

	MessageCapsule accountUpdate = MessageFactory::Empty(PresenceUpdate);
	accountUpdate->readFrom(in);
	PresenceUpdateMessage* accountupdate = dynamic_cast<PresenceUpdateMessage*>(accountUpdate.get());
	emit accountModified(accountupdate->getUserId(), accountupdate->getNickname(), accountupdate->getIcon());

}

void Client::newUserPresence(QDataStream& in) {

	MessageCapsule newAccountPresence = MessageFactory::Empty(PresenceAdd);
	newAccountPresence->readFrom(in);
	PresenceAddMessage* newaccountpresence = dynamic_cast<PresenceAddMessage*>(newAccountPresence.get());
	emit userPresence(newaccountpresence->getUserId(), newaccountpresence->getNickname(), newaccountpresence->getIcon());

}

void Client::deleteUserPresence(QDataStream& in) {

	MessageCapsule userPresence = MessageFactory::Empty(PresenceRemove);
	userPresence->readFrom(in);
	PresenceRemoveMessage* userpresence = dynamic_cast<PresenceRemoveMessage*>(userPresence.get());
	emit cancelUserPresence(userpresence->getUserId());
}

void Client::removeFromFile(qint32 myId) {

	MessageCapsule userPresence = MessageFactory::PresenceRemove(myId);
	userPresence->sendTo(socket);

}
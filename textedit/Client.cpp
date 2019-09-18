#include "Client.h"


Client::Client(QObject* parent) : QObject(parent)
{
	socket = new QSslSocket(this);

	connect(socket, SIGNAL(connected()), this, SLOT(serverConnection()));
	//connect(socket, SIGNAL(encrypted()), this, SLOT(ready()));
	connect(socket, SIGNAL(disconnected()), this, SLOT(serverDisconnection()));
	connect(socket, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(handleSslErrors(const QList<QSslError>&)));
	connect(socket, SIGNAL(error(QAbstractSocket::SocketError socketError)), this, SLOT(errorHandler(QAbstractSocket::SocketError)));
	//connect(serverSocket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error), this, &TcpServer::socketErr);

	connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
		[&](QAbstractSocket::SocketError socketError) {
			emit impossibleToConnect();						// Error in connection
			socket->abort();
		});
}


Client::~Client()
{
	// TODO
}

void Client::writeOnServer()
{

}

void Client::handleSslErrors(const QList<QSslError>& sslErrors)
{
	// Ignoring SSL errors for self-signed certificate and hostname mismatch (expected errors)
	if (sslErrors.size() == 2)
	{
		QList<QSslError>::const_iterator i = sslErrors.cbegin();
		if (i->error() == QSslError::HostNameMismatch && (i + 1)->error() == QSslError::SelfSignedCertificate)
		{
			socket->ignoreSslErrors();
		}
	}
	else
	{
		qDebug() << "Encountered SSL errors: " << sslErrors << endl;
	}
}

void Client::serverConnection() {
	qDebug() << "Connection established";
}

void Client::ready() {
	qDebug() << "Encryption ready";
	emit connectionEstablished();
}

void Client::serverDisconnection() {
	qDebug() << "Server closed the connection";
}

void Client::errorHandler(QAbstractSocket::SocketError socketError) {

	qDebug() << "Socket error raised: " << socketError;
	socket->close();
}

void Client::readBuffer() {

	qDebug() << "Reading socket";

	QByteArray dataBuffer;
	QDataStream dataStream(&(socketBuffer.buffer), QIODevice::ReadWrite);
	QDataStream in(socket);

	if (!socketBuffer.getDataSize())
	{
		// Begin reading a new message
		in >> socketBuffer;
	}

	// Read all the available message data from the socket
	dataBuffer = socket->read((qint64)(socketBuffer.getDataSize() - socketBuffer.getReadDataSize()));

	socketBuffer.append(dataBuffer);

	if (socketBuffer.bufferReadyRead()) {

		QDataStream dataStream(&(socketBuffer.buffer), QIODevice::ReadWrite);
		quint16 mType = socketBuffer.getType();

		messageHandler((MessageType)socketBuffer.getType(), dataStream);

		socketBuffer.clear();
	}
}

void Client::messageHandler(MessageType typeOfMessage, QDataStream& in) {

	switch (typeOfMessage) {
	case CursorMove:
		receiveCursor(in);
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
		receiveChar(in);
		break;
	case CharDelete:
		deleteChar(in);
		break;
	default:
		//thorw exception
		break;
	}

}

MessageCapsule Client::readMessage(QDataStream& stream, qint16 typeOfMessage)
{
	QByteArray dataBuffer;

	socketBuffer.clear();	// clear the buffer before starting reaeding
							// TODO EDO: se faccio clear alla fine di ogni lettura/errore, il clear all'inizio si potrebbe togliere?

	if (!socket->waitForReadyRead(READYREAD_TIMEOUT)) {

		switch (typeOfMessage)
		{
		case LoginMessage:
			emit loginFailed(tr("Server not responding"));
			break;
		case RegisterMessage:
			emit registrationFailed(tr("Server not responding"));
			break;
		case OpenFileMessage:
			emit openFileFailed(tr("Server not responding"));
			break;
		case CreateFileMessage:
			emit removeFileFailed(tr("Server not responding"));
			break;
		default:
			break;
		}

		socketBuffer.clear();
		return MessageCapsule();
	}

	stream >> socketBuffer;

	dataBuffer = socket->read(socketBuffer.getDataSize());

	socketBuffer.append(dataBuffer);
	/* If not all bytes were received with the first chunk, wait for the next chunks
	to arrive on the socket and append their content to the read buffer */
	while (!socketBuffer.bufferReadyRead()) {

		if (!socket->waitForReadyRead(READYREAD_TIMEOUT)) {

			switch (typeOfMessage)
			{
			case LoginMessage:
				emit loginFailed(tr("Server not responding"));
				break;
			case RegisterMessage:
				emit registrationFailed(tr("Server not responding"));
				break;
			case OpenFileMessage:
				emit openFileFailed(tr("Server not responding"));
			case CreateFileMessage:
				emit removeFileFailed(tr("Server not responding"));
			default:
				break;
			}

			socketBuffer.clear();
			return MessageCapsule();
		}

		dataBuffer = socket->read((qint64)(socketBuffer.getDataSize() - socketBuffer.getReadDataSize()));

		socketBuffer.append(dataBuffer);

	}

	// Read all the available message data from the socket

	QDataStream dataStream(&(socketBuffer.buffer), QIODevice::ReadWrite);
	MessageCapsule message = MessageFactory::Empty((MessageType)socketBuffer.getType());
	message->readFrom(dataStream);

	socketBuffer.clear();

	return message;
}

void Client::Connect(QString ipAddress, quint16 port) {
	socket->connectToHostEncrypted(ipAddress, port);
	if(socket->waitForEncrypted(READYREAD_TIMEOUT))
		ready();		// TODO EDO: si potrebbe eliminare lo slot "ready()" e mettere le istruzioni direttamente qua?
}

void Client::Disconnect() {
	socket->close();	// TODO EDO: meglio closeHostConnection?
	qDebug() << "Connection closed by client";
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

	QDataStream in(socket);
	incomingMessage = readMessage(in, LoginMessage);
	if (!incomingMessage)
		return;

	// switch to check the correctness of the type of Message
	switch (incomingMessage->getType()) {
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

	LoginChallengeMessage* loginChallenge = dynamic_cast<LoginChallengeMessage*>(incomingMessage.get());
	QByteArray nonce = loginChallenge->getNonce();
	QByteArray salt = loginChallenge->getSalt();

	qDebug() << "Cripting salt " << nonce;
	//QString result = password + salt + nonce;

	//QCryptographicHash hash(QCryptographicHash::Md5);
	//hash.addData(result.toStdString().c_str(), result.length());

	//MessageCapsule loginUnlock = MessageFactory::LoginUnlock(QString::fromStdString(hash.result().toStdString()));

	QCryptographicHash hash1(QCryptographicHash::Md5);
	QCryptographicHash hash2(QCryptographicHash::Md5);

	hash1.addData(password.toUtf8());
	hash1.addData(salt);

	hash2.addData(hash1.result());
	hash2.addData(nonce);

	MessageCapsule loginUnlock = MessageFactory::LoginUnlock(hash2.result());

	loginUnlock->sendTo(socket);

	incomingMessage = readMessage(in, LoginMessage);
	if (!incomingMessage)
		return;

	switch (incomingMessage->getType()) {
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

	QDataStream in(socket);
	MessageCapsule incomingMessage;
	// Link the stream to the socke and send the byte

	MessageCapsule accountCreate = MessageFactory::AccountCreate(username, nickname, image, password);
	accountCreate->sendTo(socket);

	//wait the response from the server
	incomingMessage = readMessage(in, RegisterMessage);
	if (!incomingMessage)
		return;

	switch (incomingMessage->getType()) {
	case AccountConfirmed: {
		AccountConfirmedMessage* accountConfirmed = dynamic_cast<AccountConfirmedMessage*>(incomingMessage.get());
		emit registrationCompleted(accountConfirmed->getUserObj());
		return;
	}
	case AccountError: {
		// impossible to create the account
		AccountErrorMessage* accountDenied = dynamic_cast<AccountErrorMessage*>(incomingMessage.get());
		emit registrationFailed(accountDenied->getErrorMessage());
		return;
	}
	default:
		//throw MessageUnknownTypeException();
		// EMIT?
		return;
	}
}

void Client::Logout() {

	/*quint16 typeOfMessage;
	QDataStream in(socket);
	MessageCapsule incomingMessage;*/

	MessageCapsule logoutRequest = MessageFactory::Logout();
	logoutRequest->sendTo(socket);
}

/*--------------------------- DOCUMENT HANDLER --------------------------------*/

void Client::openDocument(URI URI) {

	QDataStream in(socket);
	MessageCapsule incomingMessage;

	MessageCapsule openDocument = MessageFactory::DocumentOpen(URI.toString());
	openDocument->sendTo(socket);

	//wait the response from the server

	incomingMessage = readMessage(in, OpenFileMessage);
	if (!incomingMessage)
		return;

	switch (incomingMessage->getType()) {
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

	QDataStream in(socket);
	MessageCapsule incomingMessage;

	MessageCapsule newDocument = MessageFactory::DocumentCreate(name);
	newDocument->sendTo(socket);

	//wait the response from the server

	incomingMessage = readMessage(in, CreateFileMessage);
	if (!incomingMessage)
		return;

	switch (incomingMessage->getType()) {
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

void Client::deleteDocument(URI URI) {

	QDataStream in(socket);
	MessageCapsule incomingMessage;

	MessageCapsule removeDocument = MessageFactory::DocumentRemove(URI.toString());
	removeDocument->sendTo(socket);

	incomingMessage = readMessage(in, DeleteMessage);
	if (!incomingMessage)
		return;

	switch (incomingMessage->getType()) {
	case DocumentDismissed: {
		//Document successfully opened
		emit documentDismissed(URI);
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

void Client::sendCursor(qint32 userId, qint32 position) {

	// Link the stream to the socket and send the byte
	MessageCapsule moveCursor = MessageFactory::CursorMove(userId, position);
	moveCursor->sendTo(socket);
	return;
}

void Client::receiveCursor(QDataStream& in) {

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

void Client::removeChar(QVector<int> position)
{
	MessageCapsule removeChar = MessageFactory::CharDelete(position);
	removeChar->sendTo(socket);
}

void Client::receiveChar(QDataStream& in) {

	MessageCapsule reciveChar = MessageFactory::Empty(CharInsert);
	reciveChar->readFrom(in);
	CharInsertMessage* recivechar = dynamic_cast<CharInsertMessage*>(reciveChar.get());
	emit recivedSymbol(recivechar->getSymbol());
}

void Client::deleteChar(QDataStream& in) {

	MessageCapsule deleteChar = MessageFactory::Empty(CharDelete);
	deleteChar->readFrom(in);
	CharDeleteMessage* deletechar = dynamic_cast<CharDeleteMessage*>(deleteChar.get());
	emit removeSymbol(deletechar->getPosition());
}


/*--------------------------- ACCOUNT HANDLER --------------------------------*/

void Client::sendAccountUpdate(QString nickname, QImage image, QString password)
{
	QDataStream in(socket);
	QByteArray dataBuffer;

	disconnect(socket, SIGNAL(readyRead()), this, SLOT(readBuffer()));
	MessageCapsule accountUpdate = MessageFactory::AccountUpdate(nickname, image, QByteArray(password.toStdString().c_str()));
	accountUpdate->sendTo(socket);


	while (true) {

		if (!socket->waitForReadyRead(READYREAD_TIMEOUT)) {
			emit accountModificationFail(tr("Server not responding"));
			return;
		}

		in >> socketBuffer;		/* take the type of incoming message and read the size of the message */

		dataBuffer = socket->read(socketBuffer.getDataSize());		// Read all the available message data from the socket

		socketBuffer.append(dataBuffer);
		/* If not all bytes were received with the first chunk, wait for the next chunks
		to arrive on the socket and append their content to the read buffer */
		while (!socketBuffer.bufferReadyRead())
		{
			if (!socket->waitForReadyRead(READYREAD_TIMEOUT)) {
				// emit accountModificationFail(tr("Server not responding")); // che segnale emetto? teoricamente non dovrebbe mai succedere
				break;
			}
			dataBuffer = socket->read((qint64)(socketBuffer.getDataSize() - socketBuffer.getReadDataSize()));
			socketBuffer.append(dataBuffer);
		}

		if (!socketBuffer.bufferReadyRead())  // skip the message if it's not complete
			continue;

		QDataStream dataStream(&(socketBuffer.buffer), QIODevice::ReadWrite);
		switch (socketBuffer.getType()) {
		case AccountConfirmed:
		{
			MessageCapsule recivedAccountUpdate = MessageFactory::Empty(AccountConfirmed);
			recivedAccountUpdate->readFrom(dataStream);
			AccountConfirmedMessage* accountconfirmed = dynamic_cast<AccountConfirmedMessage*>(recivedAccountUpdate.get());
			connect(socket, SIGNAL(readyRead()), this, SLOT(readBuffer()));
			emit personalAccountModified(accountconfirmed->getUserObj());
			socketBuffer.clear();
			return;
		}
		case AccountError:
		{
			MessageCapsule accountError = MessageFactory::Empty(AccountError);
			accountError->readFrom(dataStream);
			AccountErrorMessage* accounterror = dynamic_cast<AccountErrorMessage*>(accountError.get());
			connect(socket, SIGNAL(readyRead()), this, SLOT(readBuffer()));
			emit accountModificationFail(accounterror->getErrorMessage());
			socketBuffer.clear();
			return;
		}
		default:
			messageHandler((MessageType)socketBuffer.getType(), dataStream);
			break;
		}

		socketBuffer.clear();
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
	disconnect(socket, SIGNAL(readyRead()), this, SLOT(readBuffer()));
}
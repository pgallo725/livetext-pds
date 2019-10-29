#include "Client.h"

#include <SharedException.h>

Client::Client(QObject* parent) : QObject(parent)
{
}


Client::~Client()
{
	// TODO
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

void Client::serverDisconnection() {
	qDebug() << "Server closed the connection";
	emit abortConnection();
	socket->abort();
	socket->deleteLater();
}

void Client::readBuffer() {

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
		MessageCapsule message = MessageFactory::Empty((MessageType)socketBuffer.getType());
		message->read(dataStream);

		messageHandler(message);

		socketBuffer.clear();
	}
}

void Client::messageHandler(MessageCapsule message) {

	switch (message->getType()) {
	case CursorMove:
		receiveCursor(message);
		break;
	case PresenceAdd:
		newUserPresence(message);
		break;
	case PresenceRemove:
		deleteUserPresence(message);
		break;
	case PresenceUpdate:
		updateUserPresence(message);
		break;
	case CharInsert:
		receiveChar(message);
		break;
	case CharDelete:
		deleteChar(message);
		break;
	case CharFormat:
		editChar(message);
		break;
	case BlockEdit:
		editBlock(message);
		break;
	case ListEdit:
		editList(message);
		break;
	case DocumentExit:
		forceDocumentClose();
		break;
	default:
		throw MessageTypeException(message->getType());
		break;
	}
}

MessageCapsule Client::readMessage(QDataStream& stream)
{
	QByteArray dataBuffer;

	if (!socket->waitForReadyRead(READYREAD_TIMEOUT)) {

		emit failureSignal(tr("Server not responding"));

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

			emit failureSignal(tr("Server not responding"));

			socketBuffer.clear();
			return MessageCapsule();
		}

		dataBuffer = socket->read((qint64)(socketBuffer.getDataSize() - socketBuffer.getReadDataSize()));

		socketBuffer.append(dataBuffer);

	}

	// Read all the available message data from the socket

	QDataStream dataStream(&(socketBuffer.buffer), QIODevice::ReadWrite);
	MessageCapsule message = MessageFactory::Empty((MessageType)socketBuffer.getType());
	message->read(dataStream);

	socketBuffer.clear();

	return message;
}

void Client::Connect(QString ipAddress, quint16 port) {

	socket = new QSslSocket(this);

	connect(socket, SIGNAL(connected()), this, SLOT(serverConnection()));
	connect(socket, SIGNAL(sslErrors(const QList<QSslError>&)), this, SLOT(handleSslErrors(const QList<QSslError>&)));
	connect(socket, SIGNAL(error(QAbstractSocket::SocketError socketError)), this, SLOT(errorHandler(QAbstractSocket::SocketError)));

	connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
		[&](QAbstractSocket::SocketError socketError) {
			emit impossibleToConnect();						// Error in connection
			socket->abort();
		});

	connect(socket, SIGNAL(disconnected()), this, SLOT(serverDisconnection()));
	socket->connectToHostEncrypted(ipAddress, port);
	if (socket->waitForEncrypted(READYREAD_TIMEOUT))
		emit connectionEstablished();
	else
		emit impossibleToConnect();
}

void Client::Disconnect() {

	disconnect(socket, SIGNAL(disconnected()), this, SLOT(serverDisconnection()));
	socket->disconnectFromHost();
	qDebug() << "Connection closed by client";
}

void Client::Login(QString usr, QString passwd) {

	MessageCapsule incomingMessage;
	MessageCapsule loginRequest = MessageFactory::LoginRequest(usr);

	loginRequest->send(socket);

	QDataStream in(socket);

	connect(this, &Client::failureSignal, this, &Client::loginFailed);
	incomingMessage = readMessage(in);
	// NOT DO: disconnect(this, &Client::failureSignal, this, &Client::loginFailed);

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
	case Failure:
	{
		emit loginFailed(tr("Server Error"));
		return;
	}
	default:
		throw MessageTypeException(incomingMessage->getType);
		return;
	}

	LoginChallengeMessage* loginChallenge = dynamic_cast<LoginChallengeMessage*>(incomingMessage.get());
	QByteArray nonce = loginChallenge->getNonce();
	QByteArray salt = loginChallenge->getSalt();

	QCryptographicHash hash1(QCryptographicHash::Sha512);
	QCryptographicHash hash2(QCryptographicHash::Sha512);

	hash1.addData(passwd.toUtf8());
	hash1.addData(salt);

	hash2.addData(hash1.result());
	hash2.addData(nonce);

	MessageCapsule loginUnlock = MessageFactory::LoginUnlock(hash2.result());

	loginUnlock->send(socket);

	// NOT DO: connect(this, &Client::failureSignal, this, &Client::loginFailed);
	incomingMessage = readMessage(in);
	disconnect(this, &Client::failureSignal, this, &Client::loginFailed);

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
	case Failure:
	{
		emit loginFailed(tr("Server Error"));
		return;
	}
	default:
		throw MessageTypeException(incomingMessage->getType);
		return;
	}
}

void Client::Register(QString usr, QString passwd, QString nick, QImage img) {

	QDataStream in(socket);
	MessageCapsule incomingMessage;
	// Link the stream to the socke and send the byte

	MessageCapsule accountCreate = MessageFactory::AccountCreate(usr, nick, img, passwd);
	accountCreate->send(socket);

	//wait the response from the server
	connect(this, &Client::failureSignal, this, &Client::registrationFailed);
	incomingMessage = readMessage(in);
	disconnect(this, &Client::failureSignal, this, &Client::registrationFailed);

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
	case Failure:
	{
		emit registrationFailed(tr("Server Error"));
		return;
	}
	default:
		throw MessageTypeException(incomingMessage->getType);
		return;
	}
}

void Client::Logout() {

	MessageCapsule logoutRequest = MessageFactory::Logout();
	logoutRequest->send(socket);

	disconnect(socket, SIGNAL(disconnected()), this, SLOT(serverDisconnection()));
	socket->disconnectFromHost();
}

/*--------------------------- DOCUMENT HANDLER --------------------------------*/

void Client::openDocument(URI URI) {

	QDataStream in(socket);
	MessageCapsule incomingMessage;

	disconnect(socket, SIGNAL(readyRead()), this, SLOT(readBuffer())); // dicconect function for Asyncronous Messages

	MessageCapsule openDocument = MessageFactory::DocumentOpen(URI.toString());
	openDocument->send(socket);

	//wait the response from the server

	connect(this, &Client::failureSignal, this, &Client::fileOperationFailed);
	incomingMessage = readMessage(in);
	disconnect(this, &Client::failureSignal, this, &Client::fileOperationFailed);

	if (!incomingMessage)
		return;

	switch (incomingMessage->getType()) {
	case DocumentReady: {
		// Open Succeded
		DocumentReadyMessage* documentOpened = dynamic_cast<DocumentReadyMessage*>(incomingMessage.get());
		connect(socket, SIGNAL(readyRead()), this, SLOT(readBuffer()));
		if (socket->encryptedBytesAvailable() > 0) 
			readBuffer();	//simulate the signal due to some byte arrived between Login and bind of the handler
		emit openFileCompleted(documentOpened->getDocument());
		return;
	}
	case DocumentError: {
		// impossible to open the Document
		DocumentErrorMessage* documentError = dynamic_cast<DocumentErrorMessage*>(incomingMessage.get());
		emit fileOperationFailed(documentError->getErrorMessage());
		return;
	}
	case Failure:
	{
		emit fileOperationFailed(tr("Server Error"));
		return;
	}
	default:
		throw MessageTypeException(incomingMessage->getType);
		return;
	}
}

void Client::createDocument(QString name) {

	QDataStream in(socket);
	MessageCapsule incomingMessage;

	disconnect(socket, SIGNAL(readyRead()), this, SLOT(readBuffer())); // dicconect function for Asyncronous Messages

	MessageCapsule newDocument = MessageFactory::DocumentCreate(name);
	newDocument->send(socket);

	//wait the response from the server

	connect(this, &Client::failureSignal, this, &Client::fileOperationFailed);
	incomingMessage = readMessage(in);
	disconnect(this, &Client::failureSignal, this, &Client::fileOperationFailed);

	if (!incomingMessage)
		return;

	switch (incomingMessage->getType()) {
	case DocumentReady: {
		//Document successfully opened
		DocumentReadyMessage* documentOpened = dynamic_cast<DocumentReadyMessage*>(incomingMessage.get());
		connect(socket, SIGNAL(readyRead()), this, SLOT(readBuffer()));
		if (socket->encryptedBytesAvailable() > 0)
			readBuffer();	//simulate the signal due to some byte arrived between Login and bind of the handler
		emit openFileCompleted(documentOpened->getDocument());
		return;
	}
	case DocumentError: {
		// impossible to open the document
		DocumentErrorMessage* documentError = dynamic_cast<DocumentErrorMessage*>(incomingMessage.get());
		emit fileOperationFailed(documentError->getErrorMessage());
		return;
	}
	case Failure:
	{
		emit fileOperationFailed(tr("Server Error"));
		return;
	}
	default:
		throw MessageTypeException(incomingMessage->getType);
		return;
	}
}

void Client::deleteDocument(URI URI) {

	QDataStream in(socket);
	MessageCapsule incomingMessage;

	disconnect(socket, SIGNAL(readyRead()), this, SLOT(readBuffer())); // dicconect function for Asyncronous Messages

	MessageCapsule removeDocument = MessageFactory::DocumentRemove(URI.toString());
	removeDocument->send(socket);

	connect(this, &Client::failureSignal, this, &Client::fileOperationFailed);
	incomingMessage = readMessage(in);
	disconnect(this, &Client::failureSignal, this, &Client::fileOperationFailed);

	if (!incomingMessage)
		return;

	switch (incomingMessage->getType()) {
	case DocumentDismissed: {
		//Document successfully removed
		emit documentDismissed(URI);
		return;
	}
	case DocumentError: {
		// impossible to remove the document
		DocumentErrorMessage* documentError = dynamic_cast<DocumentErrorMessage*>(incomingMessage.get());
		emit fileOperationFailed(documentError->getErrorMessage());
		return;
	}
	case Failure:
	{
		emit fileOperationFailed(tr("Server Error"));
		return;
	}
	default:
		throw MessageTypeException(incomingMessage->getType);
		return;
	}

}

void Client::forceDocumentClose()
{
	emit documentExitSuccess(true);
}

/*--------------------------- CURSOR HANDLER --------------------------------*/

void Client::sendCursor(qint32 userId, qint32 position) {

	// Link the stream to the socket and send the byte
	MessageCapsule moveCursor = MessageFactory::CursorMove(userId, position);
	moveCursor->send(socket);
	return;
}

void Client::receiveCursor(MessageCapsule message) {

	CursorMoveMessage* movecursor = dynamic_cast<CursorMoveMessage*>(message.get());
	emit cursorMoved(movecursor->getCursorPosition(), movecursor->getUserId());
}


/*--------------------------- CHARACTER HANDLER --------------------------------*/

void Client::sendChar(Symbol character, bool isLast) {

	MessageCapsule sendChar = MessageFactory::CharInsert(character, isLast);
	sendChar->send(socket);
}

void Client::removeChar(QVector<int> position)
{
	MessageCapsule removeChar = MessageFactory::CharDelete(position);
	removeChar->send(socket);
}

void Client::charModified(QVector<qint32> position, QTextCharFormat fmt)
{
	MessageCapsule charFormat = MessageFactory::CharFormat(position, fmt);
	charFormat->send(socket);
}

void Client::blockModified(TextBlockID blockId, QTextBlockFormat fmt)
{
	MessageCapsule blockEdit = MessageFactory::BlockEdit(blockId, fmt);
	blockEdit->send(socket);
}

void Client::listModified(TextBlockID blockId, TextListID listId, QTextListFormat fmt)
{
	MessageCapsule listEdit = MessageFactory::ListEdit(blockId, listId, fmt);
	listEdit->send(socket);
}


void Client::receiveChar(MessageCapsule message) {

	CharInsertMessage* recivechar = dynamic_cast<CharInsertMessage*>(message.get());
	emit recivedSymbol(recivechar->getSymbol(), recivechar->getIsLast());
}

void Client::deleteChar(MessageCapsule message) {

	CharDeleteMessage* deletechar = dynamic_cast<CharDeleteMessage*>(message.get());
	emit removeSymbol(deletechar->getPosition());
}

void Client::editChar(MessageCapsule message) {

	CharFormatMessage* charformat = dynamic_cast<CharFormatMessage*>(message.get());
	emit formatSymbol(charformat->getPosition(), charformat->getCharFormat());
}

void Client::editBlock(MessageCapsule message) {

	BlockEditMessage* blockedit = dynamic_cast<BlockEditMessage*>(message.get());
	emit formatBlock(blockedit->getBlockId(),blockedit->getBlockFormat());
}

void Client::editList(MessageCapsule message)
{
	ListEditMessage* listedit = dynamic_cast<ListEditMessage*>(message.get());
	emit listEditBlock(listedit->getBlockId(), listedit->getListId(), listedit->getListFormat());
}



/*--------------------------- ACCOUNT HANDLER --------------------------------*/

void Client::sendAccountUpdate(QString nickname, QImage image, QString password)
{
	QDataStream in(socket);
	QByteArray dataBuffer;
	MessageCapsule incomingMessage;
	MessageCapsule accountUpdate = MessageFactory::AccountUpdate(nickname, image, QByteArray(password.toStdString().c_str()));

	disconnect(socket, SIGNAL(readyRead()), this, SLOT(readBuffer())); // dicconect function for Asyncronous Messages
	
	accountUpdate->send(socket); //Start the Account Update sequence of messages


	while (true) {

		connect(this, &Client::failureSignal, this, &Client::accountModificationFail);
		incomingMessage = readMessage(in);
		disconnect(this, &Client::failureSignal, this, &Client::accountModificationFail);

		if (!incomingMessage)
			return;

		switch (incomingMessage->getType()) {
		case AccountConfirmed:
		{
			connect(socket, SIGNAL(readyRead()), this, SLOT(readBuffer()));
			AccountConfirmedMessage* accountconfirmed = dynamic_cast<AccountConfirmedMessage*>(incomingMessage.get());
			emit personalAccountModified(accountconfirmed->getUserObj());
			return;
		}
		case AccountError:
		{
			connect(socket, SIGNAL(readyRead()), this, SLOT(readBuffer()));
			AccountErrorMessage* accounterror = dynamic_cast<AccountErrorMessage*>(incomingMessage.get());
			emit accountModificationFail(accounterror->getErrorMessage());
			return;
		}
		case Failure:
		{
			// TODO gestione failure
		}
		default:
			messageHandler(incomingMessage);
			break;
		}
	}
}


/*--------------------------- PRESENCE HANDLER --------------------------------*/

void Client::newUserPresence(MessageCapsule message) {

	PresenceAddMessage* newaccountpresence = dynamic_cast<PresenceAddMessage*>(message.get());
	emit userPresence(newaccountpresence->getUserId(), newaccountpresence->getNickname(), newaccountpresence->getIcon());
}

void Client::deleteUserPresence(MessageCapsule message) {

	PresenceRemoveMessage* userpresence = dynamic_cast<PresenceRemoveMessage*>(message.get());
	emit cancelUserPresence(userpresence->getUserId());
}

void Client::updateUserPresence(MessageCapsule message) {

	PresenceUpdateMessage* presenceupdate = dynamic_cast<PresenceUpdateMessage*>(message.get());
	emit accountModified(presenceupdate->getUserId(), presenceupdate->getNickname(), presenceupdate->getIcon());
}

void Client::removeFromFile(qint32 myId) {

	QDataStream in(socket);
	MessageCapsule incomingMessage;
	
	MessageCapsule closeDocument = MessageFactory::DocumentClose();
	disconnect(socket, SIGNAL(readyRead()), this, SLOT(readBuffer()));
	closeDocument->send(socket);

	while (true) {

		connect(this, &Client::failureSignal, this, &Client::fileOperationFailed);
		incomingMessage = readMessage(in);
		connect(this, &Client::failureSignal, this, &Client::fileOperationFailed);
		if (!incomingMessage)
			return;

		switch (incomingMessage->getType()) {
		case DocumentExit:
		{
			DocumentExitMessage* accountconfirmed = dynamic_cast<DocumentExitMessage*>(incomingMessage.get());
			emit documentExitSuccess();
			return;
		}
		case DocumentError:
		{
			DocumentErrorMessage* accounterror = dynamic_cast<DocumentErrorMessage*>(incomingMessage.get());
			emit documentExitFailed(accounterror->getErrorMessage());
			return;
		}
		case Failure:
		{
			// TODO gestione Failure
		}
		default:
			messageHandler(incomingMessage);
			break;
		}
	}
}
#include "Client.h"

#include <SharedException.h>


Client::Client(QSharedPointer<QWaitCondition> wc, QObject* parent)
	: QObject(parent), sync(false), wc(wc)
{
	qRegisterMetaType<User>("User");
	qRegisterMetaType<Document>("Document");
	qRegisterMetaType<URI>("URI");

	// Create the client socket and attach error handler slot
	socket = new QSslSocket(this);
	connect(socket, QOverload<const QList<QSslError>&>::of(&QSslSocket::sslErrors), this, &Client::handleSslErrors);

	// Instantiate the Workspace thread and start it
	workThread = QSharedPointer<QThread>(new QThread(parent));
	connect(workThread.get(), &QThread::finished, workThread.get(), &QThread::deleteLater);
	this->moveToThread(workThread.get());
	workThread->start();
}

Client::~Client()
{
	workThread->quit();		// Quit the thread
	workThread->wait();		// Waiting for ending the thread
}


/*--------------------------- SOCKET READ AND MESSAGE HANDLERS --------------------------------*/

void Client::readBuffer() 
{
	if (socket == nullptr || !socket->isValid() || !socket->isOpen())
		return;

	QByteArray dataBuffer;
	QDataStream in(socket);

	if (!socketBuffer.getDataSize())
	{
		// Begin reading a new message
		in >> socketBuffer;
	}

	// Read all the available message data from the socket
	dataBuffer = socket->read((qint64)(socketBuffer.getDataSize() - socketBuffer.getReadDataSize()));

	socketBuffer.append(dataBuffer);

	// If the message has been completely received it, proceed to handle it
	// otherwise wait for more chunks to arrive
	if (socketBuffer.bufferReadyRead()) 
	{
		try 
		{
			QDataStream dataStream(socketBuffer.bufferPtr(), QIODevice::ReadWrite);
			MessageCapsule message = MessageFactory::Empty((MessageType)socketBuffer.getType());
			message->read(dataStream);

			messageHandler(message);

			socketBuffer.clearBuffer();
		}
		catch (MessageException& me) {
			qDebug() << me.what();
			socketBuffer.clearBuffer();
		}
	}
}


MessageCapsule Client::readMessage(QDataStream& stream)
{
	if (!socket->waitForReadyRead(READYREAD_TIMEOUT)) 
	{
		socketBuffer.clearBuffer();
		return MessageCapsule();
	}

	stream >> socketBuffer;

	QByteArray dataBuffer = socket->read(socketBuffer.getDataSize());
	socketBuffer.append(dataBuffer);

	/* If not all bytes were received with the first chunk, wait for the next chunks
	to arrive on the socket and append their content to the read buffer */
	while (!socketBuffer.bufferReadyRead()) 
	{
		if (!socket->waitForReadyRead(READYREAD_TIMEOUT)) {
			socketBuffer.clearBuffer();
			return MessageCapsule();
		}

		dataBuffer = socket->read((qint64)(socketBuffer.getDataSize() - socketBuffer.getReadDataSize()));
		socketBuffer.append(dataBuffer);
	}

	try 
	{	// Read all the available message data from the socket
		QDataStream dataStream(socketBuffer.bufferPtr(), QIODevice::ReadWrite);
		MessageCapsule message = MessageFactory::Empty((MessageType)socketBuffer.getType());
		message->read(dataStream);

		socketBuffer.clearBuffer();

		return message;
	}
	catch (MessageException& me) {
		qDebug() << me.what();
		socketBuffer.clearBuffer();
		return MessageCapsule();
	}
}


void Client::messageHandler(MessageCapsule message) 
{
	switch (message->getType()) {
	case CursorMove:
		handleCursor(message);
		break;
	case PresenceAdd:
		handleAddPresence(message);
		break;
	case PresenceRemove:
		handleRemovePresence(message);
		break;
	case PresenceUpdate:
		handleUpdatePresence(message);
		break;
	case CharsInsert:
		handleCharsInsert(message);
		break;
	case CharsDelete:
		handleCharsDelete(message);
		break;
	case CharsFormat:
		handleCharsFormat(message);
		break;
	case BlockEdit:
		handleBlockFormat(message);
		break;
	case ListEdit:
		handleListEdit(message);
		break;
	case Failure:
		disconnect(socket, &QSslSocket::readyRead, this, &Client::readBuffer);
		emit documentExitComplete(true);
		break;
	default:
		throw MessageTypeException(message->getType());
		break;
	}
}

void Client::setSync()
{
	QMutexLocker ml(&m);
	sync = true;
}

void Client::getSync()
{
	QMutexLocker ml(&m);
	if (!sync) {
		wc->wait(&m, SYNC_TIMEOUT);
		sync = true;
	}
}


/*--------------------------- CONNECTION METHODS --------------------------------*/

void Client::Connect(QString ipAddress, quint16 port) 
{
	// Attach the disconnection signal to the client slot
	connect(socket, &QSslSocket::disconnected, this, &Client::serverDisconnection);

	socket->connectToHostEncrypted(ipAddress, port);	// Attempt server connection
	if (socket->waitForEncrypted(READYREAD_TIMEOUT))
		emit connectionEstablished();
	else
	{
		// Report the connection error to the program
		QString error = socket->error() == QAbstractSocket::UnknownSocketError ?
			tr("Invalid server/port") : socket->errorString();
		emit impossibleToConnect(error);
	}
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


void Client::Disconnect() 
{
	disconnect(socket, &QSslSocket::readyRead, this, &Client::readBuffer);
	disconnect(socket, &QSslSocket::disconnected, this, &Client::serverDisconnection);
	socket->disconnectFromHost();
	qDebug() << "Closed server connection";
}


void Client::serverDisconnection()
{
	disconnect(socket, &QSslSocket::readyRead, this, &Client::readBuffer);
	disconnect(socket, &QSslSocket::disconnected, this, &Client::serverDisconnection);
	socket->abort();
	emit abortConnection();
	qDebug() << "Disconnected from server";
}



/*--------------------------- REGISTRATION/LOGIN/LOGOUT METHODS --------------------------------*/

void Client::Login(QString usr, QString passwd) 
{
	// Clear the socket from any unexpected message
	if (socket->bytesAvailable())
		socket->readAll();

	try 
	{	// Send the LoginRequest message to the server
		MessageFactory::LoginRequest(usr)->send(socket);
	}
	catch (MessageException& me) {
		qDebug() << me.what();
		emit loginFailed(tr("MessageException raised"));
		return;
	}

	QDataStream in(socket);
	MessageCapsule incomingMessage = readMessage(in);

	if (!incomingMessage)	// readMessage() returns an empty capsule in case of errors
	{
		emit loginFailed(tr("Server communication error"));
		return;
	}
		

	// Switch to check the correctness of the type of Message
	switch (incomingMessage->getType()) {
	case LoginChallenge:
	{
		// Do not return, continue with the normal code path
		break;
	}
	case LoginError:
	{
		// Server denied the login
		LoginErrorMessage* loginError = dynamic_cast<LoginErrorMessage*>(incomingMessage.get());
		emit loginFailed(loginError->getErrorMessage());
		return;
	}
	case Failure:
	{
		FailureMessage* failure = dynamic_cast<FailureMessage*>(incomingMessage.get());
		emit loginFailed(failure->getDescription());
		return;
	}
	default:
		throw MessageTypeException(incomingMessage->getType());
		return;
	}

	LoginChallengeMessage* loginChallenge = dynamic_cast<LoginChallengeMessage*>(incomingMessage.get());
	QCryptographicHash hash1(QCryptographicHash::Sha512);
	QCryptographicHash hash2(QCryptographicHash::Sha512);

	hash1.addData(passwd.toUtf8());
	hash1.addData(loginChallenge->getSalt());			// "Solve" the authentication challenge by hashing the nonce
														// with the inserted password and user's salt
	hash2.addData(hash1.result());
	hash2.addData(loginChallenge->getNonce());

	try 
	{	// Send the second message of the authentication protocol to the 
		// server, this contains the answer to the server's challenge
		MessageFactory::LoginUnlock(hash2.result())->send(socket);
	}
	catch (MessageException& we) {
		qDebug() << we.what();
		emit loginFailed(tr("MessageException raised"));
		return;
	}

	incomingMessage = readMessage(in);		// returns an empty MessageCapsule if any error occurs
	if (!incomingMessage)
	{
		emit loginFailed(tr("Server communication error"));
		return;
	}

	switch (incomingMessage->getType()) {
	case LoginGranted: 
	{
		LoginGrantedMessage* loginGranted = dynamic_cast<LoginGrantedMessage*>(incomingMessage.get());
		emit loginSuccess(loginGranted->getLoggedUser());
		return;
	}
	case LoginError: 
	{
		// Server denied the login
		LoginErrorMessage* loginerror = dynamic_cast<LoginErrorMessage*>(incomingMessage.get());
		emit loginFailed(loginerror->getErrorMessage());
		return;
	}
	case Failure:
	{
		FailureMessage* failure = dynamic_cast<FailureMessage*>(incomingMessage.get());
		emit loginFailed(failure->getDescription());
		return;
	}
	default:
		throw MessageTypeException(incomingMessage->getType());
		return;
	}
}


void Client::Register(QString usr, QString passwd, QString nick, QImage img) 
{
	QDataStream in(socket);
	MessageCapsule incomingMessage;

	// Clear the socket from any unexpected message
	if (socket->bytesAvailable())
		socket->readAll();

	try 
	{	// Send the account creation request (with all user info) to the server
		MessageFactory::AccountCreate(usr, nick, img, passwd)->send(socket);
	}
	catch (MessageException& we) {
		qDebug() << we.what();
		emit registrationFailed(tr("MessageException raised"));
		return;
	}

	// Wait for the response from the server
	incomingMessage = readMessage(in);
	if (!incomingMessage)					// returns an empty MessageCapsule if any error occurs
	{
		emit registrationFailed(tr("Server communication error"));
		return;
	}

	switch (incomingMessage->getType()) {
	case AccountConfirmed: 
	{
		AccountConfirmedMessage* accountConfirmed = dynamic_cast<AccountConfirmedMessage*>(incomingMessage.get());
		emit registrationCompleted(accountConfirmed->getUserObj());
		return;
	}
	case AccountError: 
	{
		// Server denied the account creation
		AccountErrorMessage* accountDenied = dynamic_cast<AccountErrorMessage*>(incomingMessage.get());
		emit registrationFailed(accountDenied->getErrorMessage());
		return;
	}
	case Failure:
	{
		FailureMessage* failure = dynamic_cast<FailureMessage*>(incomingMessage.get());
		emit registrationFailed(failure->getDescription());
		return;
	}
	default:
		throw MessageTypeException(incomingMessage->getType());
		return;
	}
}


void Client::Logout() {

	try 
	{	// Send the logout notification message to the server, then disconnect
		MessageFactory::Logout()->send(socket);
		Disconnect();
	}
	catch (MessageException& me) {
		qDebug() << me.what();
		emit logoutFailed(tr("MessageException raised"));
	}
}


/*--------------------------- DOCUMENT HANDLER --------------------------------*/

void Client::openDocument(URI URI) 
{
	QDataStream in(socket);
	MessageCapsule incomingMessage;

	// Clear the socket from any unexpected message
	if (socket->bytesAvailable())
		socket->readAll();

	try 
	{	// Request the document to the server
		MessageFactory::DocumentOpen(URI.toString())->send(socket);
	}
	catch (MessageException& me) {
		qDebug() << me.what();
		emit fileOperationFailed(tr("MessageException raised"));
		return;
	}
	
	// Wait the response from the server
	incomingMessage = readMessage(in);
	if (!incomingMessage)				 // returns a null MessageCapsule in case of error
	{
		emit fileOperationFailed(tr("Server communication error"));
		return;
	}

	// Check the received Message type
	switch (incomingMessage->getType()) {
	case DocumentReady: 
	{
		// Server successfully answered with the document
		DocumentReadyMessage* documentReady = dynamic_cast<DocumentReadyMessage*>(incomingMessage.get());

		//Set sync = false for the syncronization
		sync = false;
		emit openFileCompleted(documentReady->getDocument());

		//Sync between this thread and GUI thread
		getSync();

		// Connect the function which will read messages from the socket inside the editor
		connect(socket, &QSslSocket::readyRead, this, &Client::readBuffer);
		while (socket->bytesAvailable())
			readBuffer();					// Check if some bytes are already available on the socket

		return;
	}
	case DocumentError: 
	{
		// Server denied access to the document
		DocumentErrorMessage* documentError = dynamic_cast<DocumentErrorMessage*>(incomingMessage.get());
		emit fileOperationFailed(documentError->getErrorMessage());
		return;
	}
	case Failure:
	{
		FailureMessage* failure = dynamic_cast<FailureMessage*>(incomingMessage.get());
		emit fileOperationFailed(failure->getDescription());
		return;
	}
	default:
		throw MessageTypeException(incomingMessage->getType());
		return;
	}
}


void Client::createDocument(QString name) 
{
	QDataStream in(socket);
	MessageCapsule incomingMessage;

	// Clear the socket from any unexpected message
	if (socket->bytesAvailable())
		socket->readAll();

	try 
	{	// Send the document creation request to the server
		MessageFactory::DocumentCreate(name)->send(socket);
	}
	catch (MessageException& we) {
		qDebug() << we.what();
		emit fileOperationFailed(tr("MessageException raised"));
		return;
	}

	// Wait the response from the server
	incomingMessage = readMessage(in);
	if (!incomingMessage)				 // returns a null MessageCapsule in case of error
	{
		emit fileOperationFailed(tr("Server communication error"));
		return;
	}

	// Check the received Message type
	switch (incomingMessage->getType()) {
	case DocumentReady: 
	{
		//Document successfully created (and opened)
		DocumentReadyMessage* documentReady = dynamic_cast<DocumentReadyMessage*>(incomingMessage.get());

		//Set sync = false for the syncronization
		sync = false;
		emit openFileCompleted(documentReady->getDocument());

		//Sync between this thread and GUI thread
		getSync();

		connect(socket, &QSslSocket::readyRead, this, &Client::readBuffer);
		while (socket->bytesAvailable())
			readBuffer();					// Check if some bytes are already available on the socket

		return;
	}
	case DocumentError:
	{
		// Server denied the document creation
		DocumentErrorMessage* documentError = dynamic_cast<DocumentErrorMessage*>(incomingMessage.get());
		emit fileOperationFailed(documentError->getErrorMessage());
		return;
	}
	case Failure:
	{
		FailureMessage* failure = dynamic_cast<FailureMessage*>(incomingMessage.get());
		emit fileOperationFailed(failure->getDescription());
		return;
	}
	default:
		throw MessageTypeException(incomingMessage->getType());
		return;
	}
}


void Client::deleteDocument(URI URI) 
{
	QDataStream in(socket);
	MessageCapsule incomingMessage;

	// Clear the socket from any unexpected message
	if (socket->bytesAvailable())
		socket->readAll();

	try 
	{	// Send the document delete request to the server
		MessageFactory::DocumentRemove(URI.toString())->send(socket);
	}
	catch (MessageException& me) {
		qDebug() << me.what();
		emit fileOperationFailed(tr("MessageException raised"));
		return;
	}

	// Wait the response from the server
	incomingMessage = readMessage(in);
	if (!incomingMessage)				 // returns an empty MessageCapsule in case of error
	{
		emit fileOperationFailed(tr("Server communication error"));
		return;
	}

	switch (incomingMessage->getType()) {
	case DocumentDismissed: 
	{
		//Document successfully removed
		emit documentDismissed(URI);
		return;
	}
	case DocumentError: 
	{
		// Server denied document removal from account
		DocumentErrorMessage* documentError = dynamic_cast<DocumentErrorMessage*>(incomingMessage.get());
		emit fileOperationFailed(documentError->getErrorMessage());
		return;
	}
	case Failure:
	{
		FailureMessage* failure = dynamic_cast<FailureMessage*>(incomingMessage.get());
		emit fileOperationFailed(failure->getDescription());
		return;
	}
	default:
		throw MessageTypeException(incomingMessage->getType());
		return;
	}
}


void Client::closeDocument() 
{
	QDataStream in(socket);
	MessageCapsule incomingMessage;

	try 
	{	// Send the DocumentClose request to the server
		MessageFactory::DocumentClose()->send(socket);
	}
	catch (MessageException& me) {
		qDebug() << me.what();
		emit documentExitFailed(tr("Server communication error"));
		return;
	}

	// Disconnect the regular function that reads asynchronous TextEdit or Presence messages
	disconnect(socket, &QSslSocket::readyRead, this, &Client::readBuffer);

	while (true) 
	{
		// Wait the response from the server
		incomingMessage = readMessage(in);
		if (!incomingMessage)				 // returns an empty MessageCapsule if any error occurs
		{
			socket->abort();
			return;
		}

		switch (incomingMessage->getType()) {
		case DocumentExit:
		{
			// The client is allowed to close the document
			emit documentExitComplete();
			return;
		}
		case Failure:
		{
			// NOTE: Failure message is handled by messageHandler but then the loop should terminate
			messageHandler(incomingMessage);
			return;
		}
		default:
			// This operation is done while still inside the editor, therefore it's possible that other TextEdit messages
			// are arriving to the client's socket and they need to be handled properly
			messageHandler(incomingMessage);
			break;
		}
	}
}


/*--------------------------- CURSOR HANDLERS --------------------------------*/

void Client::sendCursor(qint32 userId, qint32 position) 
{
	try 
	{	// Send the cursor position to the server with CursorMove message
		MessageFactory::CursorMove(userId, position)->send(socket);
	}
	catch (MessageException& me) {
		qDebug() << me.what();
	}
}

void Client::handleCursor(MessageCapsule message) 
{
	CursorMoveMessage* movecursor = dynamic_cast<CursorMoveMessage*>(message.get());
	emit cursorMoved(movecursor->getCursorPosition(), movecursor->getUserId());
}


/*--------------------------- TEXTEDIT MESSAGE METHODS --------------------------------*/


/* Send messages to server */

void Client::sendCharsInsert(QVector<Symbol> symbols, bool isLast, TextBlockID bId, QTextBlockFormat blkFmt)
{
	try
	{
		MessageFactory::CharsInsert(symbols, isLast, bId, blkFmt)->send(socket);
	}
	catch (MessageException & me) {
		qDebug() << me.what();
	}
}

void Client::sendCharsDelete(QVector<Position> positions)
{
	try
	{
		MessageFactory::CharsDelete(positions)->send(socket);
	}
	catch (MessageException & me) {
		qDebug() << me.what();
	}
}

void Client::sendCharsFormat(QVector<Position> positions, QVector<QTextCharFormat> fmts)
{
	try 
	{
		MessageFactory::CharsFormat(positions, fmts)->send(socket);
	}
	catch (MessageException& me) {
		qDebug() << me.what();
	}
}

void Client::sendBlockFormat(TextBlockID blockId, QTextBlockFormat fmt) 
{
	try 
	{
		MessageFactory::BlockEdit(blockId, fmt)->send(socket);
	}
	catch (MessageException& me) {
		qDebug() << me.what();
	}
}

void Client::sendListEdit(TextBlockID blockId, TextListID listId, QTextListFormat fmt)
{
	try 
	{
		MessageFactory::ListEdit(blockId, listId, fmt)->send(socket);
	}
	catch (MessageException& me) {
		qDebug() << me.what();
	}
}


/* Handle messages received from server */

void Client::handleCharsInsert(MessageCapsule message)
{
	CharsInsertMessage* bulkInsertMsg = dynamic_cast<CharsInsertMessage*>(message.get());
	emit insertSymbols(bulkInsertMsg->getSymbols(), bulkInsertMsg->getIsLast(),
		bulkInsertMsg->getBlockId(), bulkInsertMsg->getBlockFormat());
}

void Client::handleCharsDelete(MessageCapsule message)
{
	CharsDeleteMessage* bulkDeleteMsg = dynamic_cast<CharsDeleteMessage*>(message.get());
	emit removeSymbols(bulkDeleteMsg->getPositions());
}

void Client::handleCharsFormat(MessageCapsule message) 
{
	CharsFormatMessage* charFormatMsg = dynamic_cast<CharsFormatMessage*>(message.get());
	emit formatSymbols(charFormatMsg->getPositions(), charFormatMsg->getCharFormats());
}

void Client::handleBlockFormat(MessageCapsule message) 
{
	BlockEditMessage* blockEditMsg = dynamic_cast<BlockEditMessage*>(message.get());
	emit formatBlock(blockEditMsg->getBlockId(), blockEditMsg->getBlockFormat());
}

void Client::handleListEdit(MessageCapsule message)
{
	ListEditMessage* listEditMsg = dynamic_cast<ListEditMessage*>(message.get());
	emit listEditBlock(listEditMsg->getBlockId(), listEditMsg->getListId(), listEditMsg->getListFormat());
}


/*--------------------------- PRESENCE HANDLERS --------------------------------*/

void Client::handleAddPresence(MessageCapsule message)
{
	PresenceAddMessage* presenceAddMsg = dynamic_cast<PresenceAddMessage*>(message.get());
	emit newUserPresence(presenceAddMsg->getUserId(), presenceAddMsg->getNickname(), presenceAddMsg->getIcon());
}

void Client::handleRemovePresence(MessageCapsule message)
{
	PresenceRemoveMessage* presenceRemoveMsg = dynamic_cast<PresenceRemoveMessage*>(message.get());
	emit removeUserPresence(presenceRemoveMsg->getUserId());
}

void Client::handleUpdatePresence(MessageCapsule message)
{
	PresenceUpdateMessage* presenceUpdateMsg = dynamic_cast<PresenceUpdateMessage*>(message.get());
	emit updateUserPresence(presenceUpdateMsg->getUserId(), presenceUpdateMsg->getNickname(),
		presenceUpdateMsg->getIcon());
}


/*--------------------------- ACCOUNT HANDLER --------------------------------*/

void Client::sendAccountUpdate(QString nickname, QImage image, QString password, bool inEditor)
{
	QDataStream in(socket);
	MessageCapsule message = MessageFactory::AccountUpdate(nickname, image, password.toUtf8());

	if (inEditor)
	{
		// Disconnect the regular function that reads asynchronous TextEdit or Presence messages
		disconnect(socket, &QSslSocket::readyRead, this, &Client::readBuffer);
	}
	else
	{
		// Clear the socket from any unexpected message
		if (socket->bytesAvailable())
			socket->readAll();
	}
	
	try 
	{	// Start the sequence of AccountUpdate messages
		message->send(socket); 
	}
	catch (MessageException& me) {
		qDebug() << me.what();
		emit accountUpdateFailed(tr("MessageException raised"));
		return;
	}

	while (true) 
	{
		// Wait the response from the server
		message = readMessage(in);
		if (!message)				 // returns a null MessageCapsule if any error occurs
		{
			emit accountUpdateFailed(tr("Server communication error"));
			return;
		}

		switch (message->getType()) {
		case AccountConfirmed:
		{
			if (inEditor) {
				connect(socket, &QSslSocket::readyRead, this, &Client::readBuffer);
			}
			AccountConfirmedMessage* accountconfirmed = dynamic_cast<AccountConfirmedMessage*>(message.get());
			emit accountUpdateComplete(accountconfirmed->getUserObj());
			return;
		}
		case AccountError:
		{
			if (inEditor) {
				connect(socket, &QSslSocket::readyRead, this, &Client::readBuffer);
			}
			AccountErrorMessage* accounterror = dynamic_cast<AccountErrorMessage*>(message.get());
			emit accountUpdateFailed(accounterror->getErrorMessage());
			return;
		}
		case Failure:
		{
			if (inEditor)
				messageHandler(message);	// NOTE: Failure message is handled by messageHandler when the TextEditor is opened
			else
			{
				FailureMessage* failure = dynamic_cast<FailureMessage*>(message.get());
				emit accountUpdateFailed(failure->getDescription());
				return;
			}
		}
		default:
		{
			// Inside the editor, handle all packets received while waiting for the response
			// When called from the LandingPage discard all unexpected messages
			if (inEditor) {
				messageHandler(message);
				break;
			}
		}
		}
	}
}

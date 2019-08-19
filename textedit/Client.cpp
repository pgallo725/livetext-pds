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
		reciveCursor();
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

void Client::setLogin(bool flag) {
	login = flag;
}

bool Client::getLogin() {
	return login;
}

QString Client::getMsg() {
	return msg_str;
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
		qDebug() << "user not exists";
		error = "user not exists";
		emit loginFailed(error);
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
		QString error = "recived no byte";
		emit loginFailed(error);
		return;
	}

	in >> typeOfMessage;

	switch (typeOfMessage) {
	case LoginAccessGranted:
		break;
	case LoginError:
		// user not exist
		qDebug() << "user not exists";
		error = "user not exists";
		emit loginFailed(error);
		return;
		break;
	default:
		//throw MessageUnknownTypeException();
		//EMIT ?
		return;
		break;
	}

	LoginMessage* loginAccess = new LoginMessage(LoginAccessGranted);
	loginAccess->readFrom(in);
	emit loginSuccess();
	return;
	
}


void Client::Register() {


	quint16 typeOfMessage;
	QDataStream in;
	QImage image;
	QString error;
	// Link the stream to the socke and send the byte

	in.setDevice(socket);

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
		qDebug() << msg_str;
		error = "error during creation";
		emit registrationFailed(error);
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
	quint16 typeOfMessage = LogoutRequest;
	QDataStream in;

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

	in.setDevice(socket);
	in >> typeOfMessage;

	switch (typeOfMessage) {
	case LogoutConfirmed:

		qDebug() << msg_str;
		MessageCapsule logoutConfirmed = new LogoutMessage(LogoutConfirmed);
		logoutConfirmed->readFrom(in);

		return;
		break;
	case LogoutDenied:
		// impossible to create the account
		qDebug() << msg_str;
		error = "Logout Denied";
		emit logoutFailed(error);
		return;
		break;
	default:
		//throw MessageUnknownTypeException();
		// EMIT?
		return;
		break;
	}

}

void Client::sendCursor(int position) {

	QDataStream out;
	quint16 typeOfMessage = MoveCursor;

	// Link the stream to the socke and send the byte
	// TODO creazione e invio messaggio

	return;
}

void Client::reciveCursor() {
	
	QDataStream in;
	int position;
	QString user;

	in >> position >> user; // TODO sostituire con messaggio

	emit cursorMoved(position, user);

}



void Client::sendChar() {


}

void Client::reciveChar() {

}
void Client::openDocument(QString URI) {

	QDataStream out;
	quint16 typeOfMessage = OpenDocument;
	QDataStream in;


	// Link the stream to the socke and send the byte
	out.setDevice(socket);
	out << typeOfMessage << URI;

	//wait the response from the server
	if (!socket->waitForReadyRead(10000)) {
		qDebug() << "recived no byte";
		//throw ServerNotRespondException();
		return;
	}

	in.setDevice(socket);
	in >> typeOfMessage;
	in >> msg_str;

	switch (typeOfMessage) {
	case DocumentOpened:
		qDebug() << msg_str;
		connect(socket, SIGNAL(readyRead()), this, SLOT(readBuffer()));
		return;
		break;
	case DocumentError:
		// impossible to create the account
		qDebug() << msg_str;
		return;
		break;
	default:
		//throw MessageUnknownTypeException();
		return;
		break;
	}
}

void Client::createDocument(QString name) {

	QDataStream out;
	quint16 typeOfMessage = NewDocument;
	QDataStream in;


	// Link the stream to the socke and send the byte
	out.setDevice(socket);
	out << typeOfMessage << name;

	//wait the response from the server
	if (!socket->waitForReadyRead(10000)) {
		qDebug() << "recived no byte";
		//throw ServerNotRespondException();
		return;
	}

	in.setDevice(socket);
	in >> typeOfMessage;
	in >> msg_str;

	switch (typeOfMessage) {
	case DocumentOpened:
		qDebug() << msg_str;
		connect(socket, SIGNAL(readyRead()), this, SLOT(readBuffer()));
		return;
		break;
	case DocumentError:
		// impossible to create the account
		qDebug() << msg_str;
		return;
		break;
	default:
		//throw MessageUnknownTypeException();
		return;
		break;
	}

}

void Client::requestURI() {

}
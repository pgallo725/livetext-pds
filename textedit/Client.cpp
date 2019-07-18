#include "Client.h"

Client::Client(QObject* parent) : QObject(parent)
{
	socket = new QTcpSocket(this);

	connect(socket, SIGNAL(connected()), this, SLOT(serverConnection()));
	connect(socket, SIGNAL(disconnected()), this, SLOT(serverDisconnection()));
	//connect(socket, SIGNAL(readyRead()), this, SLOT(readBuffer()));
	connect(socket, SIGNAL(error(QAbstractSocket::SocketError socketError)), this, SLOT(errorHandle()));

	connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
		[&](QAbstractSocket::SocketError socketError) { qDebug() << "non funziona un cazzo";
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

	QByteArray responce = socket->read(256);
	qDebug() << responce;
}
void Client::Connect(QString ipAddress, quint16 port) {
	//TODO GESTIONE DATI ERRATI
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

bool Client::Login() {
	
	QDataStream out;
	quint16 typeOfMessage=LoginRequest;

	out.setDevice(socket);
	out << typeOfMessage << username;

	QDataStream in;
	if (!socket->waitForReadyRead(10000)) {
		qDebug() << "recived no byte";
		//throw ServerNotRespondException();
		return false;
	}

	in.setDevice(socket);
	in >> typeOfMessage;

	switch (typeOfMessage) {
	case LoginChallenge:
		break; 
	case LoginError:
		// user not exist
		qDebug() << "user not exists";
		return false;
		break;
	default:
		//throw MessageUnknownTypeException();
		return false;
		break;
	}

	QCryptographicHash hash(QCryptographicHash::Md5);
	QString salt;

	in >> salt;

	typeOfMessage = LoginUnlock;
	qDebug() << "cripting salt " << salt;
	password +=  salt;
	hash.addData(password.toStdString().c_str(), password.length());

	out << typeOfMessage << QString::fromStdString(hash.result().toStdString());
	
	if (!socket->waitForReadyRead(10000)) {
		qDebug() << "recived no byte";
		//throw ServerNotRespondException();
		return false;
	}

	in >> typeOfMessage;
	in >> msg_str;
	if (typeOfMessage == LoginAccessGranted) {
		qDebug() << msg_str;
		return true;
	}
	
	qDebug() << msg_str;
	socket->close();
	return false;
}


bool Client::Registre() {

	QDataStream out;
	quint16 typeOfMessage = AccountCreate;
	QDataStream in;
	
	// Link the stream to the socke and send the byte
	out.setDevice(socket);
	out << typeOfMessage << username << nickname << password;

	//wait the response from the server
	if (!socket->waitForReadyRead(10000)) {
		qDebug() << "recived no byte";
		//throw ServerNotRespondException();
		return false;
	}

	in.setDevice(socket);
	in >> typeOfMessage;
	in >> msg_str;

	switch (typeOfMessage) {
	case AccountConfirmed:
		qDebug() << msg_str;
		return true;
		break;
	case AccountDenied:
		// impossible to create the account
		qDebug() << msg_str;
		return false;
		break;
	default:
		//throw MessageUnknownTypeException();
		return false;
		break;
	}
}
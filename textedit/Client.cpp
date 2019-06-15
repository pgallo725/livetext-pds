#include "Client.h"


Client::Client(QObject* parent) : QObject(parent)
{
	socket = new QTcpSocket(this);

	connect(socket, SIGNAL(connected()), this, SLOT(serverConnection()));
	connect(socket, SIGNAL(disconnected()), this, SLOT(serverDisconnection()));
	//connect(socket, SIGNAL(readyRead()), this, SLOT(readBuffer()));

	socket->connectToHost("127.0.0.1", 9999);
}


Client::~Client()
{
	// TODO
}

void Client::writeOnServer()
{


	qDebug() << "- send msg to the server";

	socket->flush();
	QString a, b, c;
	int x = 23423;
	a = "edo ";
	b = "e'";
	c = " bravo";
	std::stringstream ss;

	//ss << a << x << b << c;


	QString s = QString::fromStdString(ss.str());
	QTextStream os(socket);
	QByteArray buff;
	QDataStream stream(&buff, QIODevice::WriteOnly);


	stream << a << x << b;

	socket->write(buff);

	socket->flush();

	socket->waitForBytesWritten(3000);


}

void Client::serverConnection() {
	qDebug() << "Client connesso al server ";
	//writeOnServer();
	//writeOnServer();
}

void Client::serverDisconnection() {
	//socket->close();
	qDebug() << "Client disconnesso dal server ";
}

void Client::readBuffer() {

	QByteArray responce = socket->read(256);
	qDebug() << responce;



	//socket->close();
}

bool Client::Login(QString username, QString password) {
	
	QByteArray sentData;
	QDataStream stream(&sentData, QIODevice::WriteOnly);
	quint16 type=1;

	stream << type << username;

	socket->write(sentData);
	socket->flush();

	QDataStream in;
	socket->waitForReadyRead(10000); // remove whene created exception
	//if (!socket->waitForBytesWritten(10000))
		//throw ServerNotRespondException();

	in.setDevice(socket);
	quint16 type1=-1;

	in >> type1;

	switch (type1) {
	case 0:
		{ QString salt;
		in >> salt;
		//password = salt + password + salt;
		//std::size_t hashed = std::hash<std::string>{}(password.toStdString);
		stream << username << password;
		socket->write(sentData);
		socket->flush();
		return true;
		break; }
	case 1:
		return false;
		break;
	default:
		//throw MessageUnknownTypeException();
		return false;
		break;
	}

	return true;
}

bool Client::Connect(QString ipAddress, quint16 port) {

	//socket->connectToHost(ipAddress, port);
	if (socket->state() != QAbstractSocket::ConnectedState)
	{
		qDebug() << "error connecting";
		return false;
	}
	return true;
}
#include "TcpServer.h"
#include <string>

TcpServer::TcpServer(QObject* parent) : QObject(parent) {
	/* create a new object TCP server */
	textServer = new QTcpServer(this);
	
	/* connect newConnection from QTcpServer to newClientConnection() */
	connect(textServer, SIGNAL(newConnection()), this, SLOT(newClientConnection()));

	/* server listen on 0.0.0.0:9999 - return true on success */
	if (!textServer->listen(QHostAddress::Any, 9999))
	{
		qDebug() << "Server could not start";
	}
	else
	{
		/* take ip address and port */
		QString ip_address = textServer->serverAddress().toString();
		quint16 port = textServer->serverPort();
		qDebug() << "Server started at "<< ip_address <<":"<< port;
	}
}

TcpServer::~TcpServer()
{
	// TODO
}

/* handle a new connection from a client */
void TcpServer::newClientConnection()
{
	/* need to grab the socket - socket is created as a child of server */
	QTcpSocket* socket = textServer->nextPendingConnection();

	/* check if there's a new connection or it was a windows signal */
	if (socket == 0) {
		qDebug() << "ERROR: no pending connections!\n";
	}

	qDebug() << " - new connection from a client";

	/* write on socket */
	socket->write("Hello client\r\n");
	/* forse the socket to send all the data stored */
	socket->flush();

	/* This function blocks until at least one byte has been written on the socket */
	socket->waitForBytesWritten(3000);

	/* This function blocks until new data is available for reading and the readyRead() signal has been emitted
	 * returns true if theres a new data */
	if (socket->waitForReadyRead(10000)) {
		QByteArray r = socket->readLine(30);
		qDebug() << r;
	}

}

void TcpServer::clientDisconnection()
{
	/* take the object (socket) where the signal was send */
	QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
	socket->close();
}
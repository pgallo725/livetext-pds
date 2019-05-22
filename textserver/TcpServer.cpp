#include "TcpServer.h"
#include <string>

TcpServer::TcpServer(QObject* parent) : QObject(parent) {
	/* create a new object TCP server */
	textServer = new QTcpServer(this);
	
	/*        sender             signal        receiver      method      */
	connect(textServer, SIGNAL(newConnection()), this, SLOT(newConnection()));

	/* server listen on 0.0.0.0:9999 - return true on success */
	if (!textServer->listen(QHostAddress::Any, 9999))
	{
		qDebug() << "Server could not start";
	}
	else
	{
		/* take ip and port */
		QString ip = textServer->serverAddress().toString();
		quint16 port = textServer->serverPort();
		qDebug() << "Server started at "<< ip <<":"<< port;
	}
}

TcpServer::~TcpServer()
{
}

/* it's a signal emitted every time a new connection is available */
void TcpServer::newConnection()
{
	// need to grab the socket - socket is created as a child of server
	QTcpSocket* socket = textServer->nextPendingConnection();

	if (socket == 0) {
		qDebug() << "ERROR: no pending connections!\n";
	}

	/* write on socket */
	socket->write("Hello client\r\n");
	/* This function writes as much as possible from the internal write buffer to the underlying network socket, 
	 * without blocking. If any data was written, this function returns true */
	socket->flush();

	/* This function blocks until at least one byte has been written on the socket 
	 * return true if bytesWritten is written */
	socket->waitForBytesWritten(3000);

	/* This function blocks until new data is available for reading and the readyRead() signal has been emitted
	 * returns true if theres a new data */
	if (socket->waitForReadyRead(10000)) {
		QByteArray r = socket->readLine(30);
		qDebug() << r;
	}

	socket->close();
}

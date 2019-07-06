#pragma once

#include <qobject.h>
#include <qtcpsocket.h>
#include <QtNetwork>

#include <string>

#include <fstream>
#include <iostream>
#include <sstream>


class Client : public QObject
{
	Q_OBJECT
private:
	QTcpSocket* socket;
public:
	Client(QObject* parent = 0);
	~Client();
	void writeOnServer();
public slots:
	void serverConnection();
	void readBuffer();
	void serverDisconnection();
	bool Login(QString username,QString password);
	bool Connect(QString ipAddress, quint16 port);
};


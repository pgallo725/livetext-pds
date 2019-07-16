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
	QString username;
	QString password;
public:
	Client(QObject* parent = 0);
	~Client();
	void writeOnServer();
public slots:
	void serverConnection();
	void readBuffer();
	void serverDisconnection();
	bool Login();
	bool Connect(QString ipAddress, quint16 port);
	void Disconnect();
	void setUsername(QString username);
	void setPassword(QString password);

};


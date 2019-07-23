#pragma once

#include <qobject.h>
#include <qtcpsocket.h>
#include <QtNetwork>
#include <QObject>

#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <csignal>
#include <stdio.h>

#include <string>

#include <fstream>
#include <iostream>
#include <sstream>


#include "..//textserver/AccountMessage.h"
#include "..//textserver/Message.h"
#include "..//textserver/LoginMessage.h"
#include "..//textserver/LogoutMessage.h"

class Client : public QObject
{
	Q_OBJECT
private:
	QTcpSocket* socket;
	QString username;
	QString nickname;
	QString password;
	QString msg_str; // message recived by the client
	bool login;
signals:
	void connectionEstablished();
	void impossibleToConnect();
public:
	Client(QObject* parent = 0);
	~Client();
	void writeOnServer();

public slots:
	//signals
	void serverConnection();
	void readBuffer();
	void serverDisconnection();
	void errorHandler();
	// User connection
	bool Login();
	bool Register();
	bool Logout();
	//Data Exchange
	bool sendCursor();
	void reciveCursor();
	void sendChar();
	void reciveChar();
	void openDocument(QString URI);
	void createDocument(QString name);
	//Server connection
	void Connect(QString ipAddress, quint16 port);
	void Disconnect();
	// Setter & Getter
	void setUsername(QString username);
	void setPassword(QString password);
	void setLogin(bool flag);
	void setNickname(QString nickname);
	bool getLogin();
	QString getMsg();
};


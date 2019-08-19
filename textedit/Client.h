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


#include "../textserver/AccountMessage.h"
#include "../textserver/Message.h"
#include "../textserver/LoginMessage.h"
#include "../textserver/LogoutMessage.h"
#include "../textserver/User.h"

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
	void cursorMoved(int position, QString user);
	void loginSuccess();
	void loginFailed(QString errorType);
	void registrationCompleted();
	void registrationFailed(QString errorType);
	void logoutComplited();
	void logoutFailed(QString errorType);

public:
	Client(QObject* parent = 0);
	~Client();

public slots:
	//signals handler
	void serverConnection();
	void readBuffer();
	void serverDisconnection();
	void errorHandler();
	void writeOnServer();
	// User connection
	void Login();
	void Register();
	void Logout();
	//Data Exchange
	void sendCursor(int position);
	void reciveCursor();
	void sendChar();
	void reciveChar();
	void openDocument(QString URI);
	void createDocument(QString name);
	void requestURI();
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


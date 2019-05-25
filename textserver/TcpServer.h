#pragma once
#include <qobject.h>
#include <qtcpserver.h>
#include <qtcpsocket.h>
#include <map>

#include "User.h"

class TcpServer : public QObject {
	Q_OBJECT
private:
	QTcpServer* textServer;
	std::map<std::string, User> users;
public:
	TcpServer(QObject *parent = 0);
	~TcpServer();

public slots:
	void newClientConnection();
	void clientDisconnection();
};


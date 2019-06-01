#pragma once
#include <qobject.h>
#include <qtcpserver.h>
#include <qtcpsocket.h>
#include <map>
#include <optional>

#include "User.h"
#include "Document.h"
#include "WorkSpace.h"

#define TIMEOUT 15000 /* ms */

class TcpServer : public QObject {
	Q_OBJECT
private:
	QTcpServer* textServer;
	std::map<std::string, User> users;
	std::map<std::string, Document> documents;
	std::map<std::string, WorkSpace> workspaces;
	int _userIdCounter;

	bool login(std::string username, std::string passwd);
	std::optional<User> createNewAccount(std::string userName, std::string name, std::string suername, std::string passwd);
public:
	TcpServer(QObject *parent = 0);
	~TcpServer();
	
public slots:
	void newClientConnection();
	void clientDisconnection();
};

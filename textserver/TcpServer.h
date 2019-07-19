#pragma once

#include <optional>
#include <memory>

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>

#include "User.h"
#include "Client.h"
#include "Document.h"
#include "WorkSpace.h"
#include "Message.h"
#include "LoginMessage.h"
#include "AccountMessage.h"
#include "LogoutMessage.h"
#include "DocumentMessage.h"

#define TIMEOUT 15000 /* ms */

#define USERS_FILENAME "users.dat"
#define TMP_USERS_FILENAME "users.tmp"


class TcpServer : public QTcpServer
{
	Q_OBJECT

private:

	QMap<QString, User> users;
	QMap<int, Document> documents;
	QMap<int, WorkSpace> workspaces;
	QMap<QTcpSocket*, Client> clients;
	int _userIdCounter;

	bool login(Client client, QString password);
	std::optional<User> createNewAccount(QString userName, QString nickname, QString passwd, QTcpSocket *socket = nullptr);

	void saveUsers();
	void handleMessage(std::shared_ptr<Message> msg, QTcpSocket* socket);
	void sendLoginChallenge(QTcpSocket* socket, QString username);
public:

	TcpServer(QObject *parent = 0);
	~TcpServer();

	void initialize();
	
public slots:
	void newClientConnection();
	void clientDisconnection();
	void readMessage();
};

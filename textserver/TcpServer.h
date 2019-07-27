#pragma once

#include <optional>
#include <memory>

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QThread>

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

Q_DECLARE_METATYPE(qintptr);

class TcpServer : public QTcpServer
{
	Q_OBJECT
private:

	QMap<QString, User> users;
	QMap<QString, QSharedPointer<Document>> documents;
	QMap<QString, QSharedPointer<WorkSpace>> workspaces;
	QMap<QString, QSharedPointer<QThread>> workThreads;
	QMap<QTcpSocket*, QSharedPointer<Client>> clients;
	int _userIdCounter;

	bool login(QSharedPointer<Client> client, QString password);
	std::optional<User> createNewAccount(QString userName, QString nickname, QString passwd, QTcpSocket *socket = nullptr);

	void saveUsers();
	void handleMessage(std::unique_ptr<Message>&& msg, QTcpSocket* socket);
	void sendLoginChallenge(QTcpSocket* socket, QString username);
	bool createNewDocument(QString documentName, QString uri, QTcpSocket* author);
public:

	TcpServer(QObject *parent = 0);
	~TcpServer();

	void initialize();
	
public slots:
	void newClientConnection();
	void clientDisconnection();
	void readMessage();
	void deleteWorkspace();
signals:
	void newSocket(qint64 handle);
};

#pragma once

#include <optional>
#include <memory>

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QThread>
#include <QStringList>

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

	/* methods */
	bool login(QSharedPointer<Client> client, QString password);
	bool logout(QTcpSocket* s);
	bool createNewAccount(QString userName, QString nickname, QString passwd, QPixmap icon, QTcpSocket *socket = nullptr);
	bool updateAccount(User* user, quint16 typeField, QVariant newField);
	void saveUsers();
	void handleMessage(std::unique_ptr<Message>&& msg, QTcpSocket* socket);
	void sendLoginChallenge(QTcpSocket* socket, QString username);
	bool createNewDocument(QString documentName, QString uri, QTcpSocket* author);
	bool openDocument(QString uri, QTcpSocket* client);
	QStringList getUriFromUser(QString username);

public:

	TcpServer(QObject *parent = 0);
	~TcpServer();

	void initialize();
	QSharedPointer<Client> getClient(qintptr socketDescriptor);

public slots:

	void newClientConnection();
	void clientDisconnection();
	void readMessage();
	void deleteWorkspace(QString document);

signals:

	void newSocket(qint64 handle);
};

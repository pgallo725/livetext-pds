#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QThread>
#include <QTimer>

#include "User.h"
#include "Client.h"
#include "Document.h"
#include "WorkSpace.h"
#include "Message.h"
#include "LoginMessage.h"
#include "AccountMessage.h"
#include "LogoutMessage.h"
#include "DocumentMessage.h"
#include "Messagehandler.h"

#define CONNECT_TIMEOUT 15000	/* ms */
#define SAVE_TIMEOUT 10000		/* ms */
#define INDEX_FILENAME "documents.dat"
#define USERS_FILENAME "users.dat"
#define TMP_USERS_FILENAME "users.tmp"

Q_DECLARE_METATYPE(qintptr);


class TcpServer : public QTcpServer
{
	Q_OBJECT

	friend class MessageHandler;

private:

	QMap<QString, User> users;
	QMap<QString, QSharedPointer<Document>> documents;
	QMap<QString, QSharedPointer<WorkSpace>> workspaces;	// TODO(?): can workspaces and workThreads be unified ?
	//QMap<QString, QSharedPointer<QThread>> workThreads;
	QMap<QTcpSocket*, QSharedPointer<Client>> clients;
	qint32 _userIdCounter;

	QTimer time;

	MessageHandler messageHandler;

	QString generateURI(QString authorName, QString docName) const;

public:

	TcpServer(QObject *parent = 0);
	~TcpServer();

	void initialize();
	QSharedPointer<Client> moveClient(qintptr socketDescriptor);

public slots:

	void saveUsers();
	void newClientConnection();
	void clientDisconnection();
	void readMessage();
	WorkSpace* createWorkspace(QSharedPointer<Document> document, QSharedPointer<Client> client);
	void deleteWorkspace(QString document);

	MessageCapsule serveLoginRequest(QTcpSocket* socket, QString username);
	MessageCapsule authenticateUser(QTcpSocket* clientSocket, QString token);

	MessageCapsule createAccount(QTcpSocket* clientSocket, User& newUser);
	MessageCapsule updateAccount(QTcpSocket* clientSocket, User& updatedUser);

	MessageCapsule removeDocument(QTcpSocket* client, QString docUri);
	MessageCapsule createDocument(QTcpSocket* author, QString docName);
	MessageCapsule openDocument(QTcpSocket* clientSocket, QString docUri);

	MessageCapsule logoutClient(QTcpSocket* clientSocket);
	
signals:

	void newSocket(qint64 handle);
};

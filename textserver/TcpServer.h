#pragma once

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QString>
#include <QTimer>

#include <User.h>
#include "Client.h"
#include <Document.h>
#include "WorkSpace.h"
#include <Message.h>
#include "MessageHandler.h"

#define CONNECT_TIMEOUT 15000		/* ms */
#define USERS_SAVE_TIMEOUT 10000	/* ms */

Q_DECLARE_METATYPE(qintptr);
Q_DECLARE_METATYPE(URI);


class TcpServer : public QTcpServer
{
	Q_OBJECT

	friend class MessageHandler;

private:

	QMap<QString, User> users;
	QMap<URI, QSharedPointer<Document>> documents;
	QMap<URI, QSharedPointer<WorkSpace>> workspaces;
	QMap<QTcpSocket*, QSharedPointer<Client>> clients;
	qint32 _userIdCounter;

	QTimer time;

	MessageHandler messageHandler;

	QMutex users_mutex;

	URI generateURI(QString authorName, QString docName) const;

public:

	TcpServer(QObject *parent = 0);
	~TcpServer();

	void initialize();

public slots:

	void saveUsers();
	void newClientConnection();
	void clientDisconnection();
	void readMessage();
	WorkSpace* createWorkspace(QSharedPointer<Document> document, QSharedPointer<Client> client);
	void deleteWorkspace(URI document);

	MessageCapsule serveLoginRequest(QTcpSocket* socket, QString username);
	MessageCapsule authenticateUser(QTcpSocket* clientSocket, QByteArray token);

	MessageCapsule createAccount(QTcpSocket* clientSocket, QString username, QString nickname, QImage icon, QString password);
	MessageCapsule updateAccount(QTcpSocket* clientSocket, QString nickname, QImage icon, QString password);

	MessageCapsule removeDocument(QTcpSocket* client, URI docUri);
	MessageCapsule createDocument(QTcpSocket* author, QString docName);
	MessageCapsule openDocument(QTcpSocket* clientSocket, URI docUri);

	void logoutClient(QTcpSocket* clientSocket);
	void receiveClient(QSharedPointer<Client> client);
	
signals: void newSocket(qint64 handle);
signals: void clientToWorkspace(QSharedPointer<Client> client);

};

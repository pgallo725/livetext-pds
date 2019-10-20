#pragma once

#include <QObject>
#include <QTcpServer>
#include <QSslSocket>
#include <QSslConfiguration>

#include <User.h>
#include "Client.h"
#include <Document.h>
#include "WorkSpace.h"
#include "ServerDatabase.h"
#include <Message.h>
#include "MessageHandler.h"
#include "SocketBuffer.h"



class TcpServer : public QTcpServer
{
	Q_OBJECT

	friend class MessageHandler;

private:

	ServerDatabase db;

	QMap<QString, User> users;
	QStringList usersNotAvailable;
	qint32 _userIdCounter;

	QMap<URI, QSharedPointer<Document>> documents;
	QMap<URI, QSharedPointer<WorkSpace>> workspaces;
	QMap<QSslSocket*, QSharedPointer<Client>> clients;
	
	MessageHandler messageHandler;

	SocketBuffer socketBuffer;

	QSslConfiguration config;


	URI generateURI(QString authorName, QString docName) const;
	bool validateURI(URI uri) const;

public:

	TcpServer(QObject *parent = 0);
	~TcpServer();

	void initialize();

public slots:

	void newClientConnection();
	void clientDisconnection();
	void readMessage();
	QSharedPointer<WorkSpace> createWorkspace(QSharedPointer<Document> document);
	void deleteWorkspace(URI document);

	void incomingConnection(qintptr handle) Q_DECL_OVERRIDE;
	void sslSocketError(QAbstractSocket::SocketError socketError);
	void sslSocketReady();

	MessageCapsule serveLoginRequest(QSslSocket* socket, QString username);
	MessageCapsule authenticateUser(QSslSocket* clientSocket, QByteArray token);

	MessageCapsule createAccount(QSslSocket* clientSocket, QString username, QString nickname, QImage icon, QString password);
	MessageCapsule updateAccount(QSslSocket* clientSocket, QString nickname, QImage icon, QString password);
	void workspaceAccountUpdate(QSharedPointer<Client> client, QString nickname, QImage icon, QString password);

	MessageCapsule removeDocument(QSslSocket* client, URI docUri);
	MessageCapsule createDocument(QSslSocket* author, QString docName);
	MessageCapsule openDocument(QSslSocket* clientSocket, URI docUri, bool docJustCreated = false);

	void receiveClient(QSharedPointer<Client> client);
	void logoutClient(QSslSocket* clientSocket);
	void restoreUserAvaiable(QString username);


signals: void newSocket(qint64 handle);
signals: void clientToWorkspace(QSharedPointer<Client> client);
signals: void sendAccountUpdate(QSharedPointer<Client> client, MessageCapsule msg);

};

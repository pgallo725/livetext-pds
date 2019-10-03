#pragma once

#include <QObject>
#include <QTcpServer>
#include <QSslSocket>
#include <QSslConfiguration>
#include <QSaveFile>

#include <User.h>
#include "Client.h"
#include <Document.h>
#include "WorkSpace.h"
#include <Message.h>
#include "MessageHandler.h"
#include "SocketBuffer.h"



class TcpServer : public QTcpServer
{
	Q_OBJECT

	friend class MessageHandler;
	QSslConfiguration config;

private:

	QMap<QString, User> users;
	QMap<URI, QSharedPointer<Document>> documents;
	QMap<URI, QSharedPointer<WorkSpace>> workspaces;
	QMap<QSslSocket*, QSharedPointer<Client>> clients;
	QStringList usersNotAvaiable;
	qint32 _userIdCounter;
	QSaveFile usersFile;
	QSaveFile docsFile;

	MessageHandler messageHandler;

	SocketBuffer socketBuffer;

	URI generateURI(QString authorName, QString docName) const;
	bool validateURI(URI uri) const;

	void TcpServer::saveDocIndex();

public:

	TcpServer(QObject *parent = 0);
	~TcpServer();

	void initialize();

public slots:

	void saveUsers();
	void newClientConnection();
	void clientDisconnection();
	void readMessage();
	QSharedPointer<WorkSpace> createWorkspace(QSharedPointer<Document> document, QSharedPointer<Client> client);
	void deleteWorkspace(URI document);

	MessageCapsule serveLoginRequest(QSslSocket* socket, QString username);
	MessageCapsule authenticateUser(QSslSocket* clientSocket, QByteArray token);

	MessageCapsule createAccount(QSslSocket* clientSocket, QString username, QString nickname, QImage icon, QString password);
	MessageCapsule updateAccount(QSslSocket* clientSocket, QString nickname, QImage icon, QString password);
	MessageCapsule accountUpdate(User* user, QString nickname, QImage icon, QString password);

	MessageCapsule removeDocument(QSslSocket* client, URI docUri);
	MessageCapsule createDocument(QSslSocket* author, QString docName);
	MessageCapsule openDocument(QSslSocket* clientSocket, URI docUri);

	void logoutClient(QSslSocket* clientSocket);
	void receiveClient(QSharedPointer<Client> client);
	void receiveUpdateAccount(QSharedPointer<Client> client, QString nickname, QImage icon, QString password);

	void restoreUserAvaiable(QString username);

	void incomingConnection(qintptr handle) Q_DECL_OVERRIDE;
	void socketErr(QAbstractSocket::SocketError socketError);
	void ready();


signals: void newSocket(qint64 handle);
signals: void clientToWorkspace(QSharedPointer<Client> client);
signals: void sendAccountUpdate(QSharedPointer<Client> client, MessageCapsule msg);

};

#pragma once

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QMutex>
#include <QSslSocket>

#include <Document.h>
#include "Client.h"
#include "MessageHandler.h"
#include "ServerException.h"
#include "SocketBuffer.h"

#define DOCUMENT_SAVE_TIMEOUT 5000	/* ms */

class WorkSpace : public QObject
{
	Q_OBJECT

	friend class MessageHandler;

private:

	QSharedPointer<Document> doc;
	QSharedPointer<QThread> workThread;
	QMap<QSslSocket*, QSharedPointer<Client>> editors;

	QTimer timer;

	MessageHandler messageHandler;

	SocketBuffer socketBuffer;

public:

	WorkSpace(QSharedPointer<Document> d, /*QMutex& m,*/ QObject* parent = 0);
	~WorkSpace();

public slots:

	void newClient(QSharedPointer<Client> client);
	void clientDisconnection();
	void readMessage();
	
	void documentSave();
	void documentInsertSymbol(Symbol& symbol);
	void documentDeleteSymbol(QVector<qint32> position);

	void dispatchMessage(MessageCapsule message, QSslSocket* sender);

	void updateAccount(QSslSocket* clientSocket, QString nickname, QImage icon, QString password);
	void receiveUpdateAccount(QSharedPointer<Client> client, MessageCapsule msg);

	void clientQuit(QSslSocket* clientSocket);

	void socketErr(QAbstractSocket::SocketError socketError);

signals: void noEditors(URI documentURI);
signals: void returnClient(QSharedPointer<Client> client);
signals: void restoreUserAvaiable(QString username);
signals: void sendAccountUpdate(QSharedPointer<Client> client, QString nickname, QImage icon, QString password);
};


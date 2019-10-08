#pragma once

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QSslSocket>

#include <Document.h>
#include "Client.h"
#include "MessageHandler.h"
#include "ServerException.h"
#include "SocketBuffer.h"

#define DOCUMENT_SAVE_TIMEOUT 30000		/* ms */
#define DOCUMENT_MAX_FAILS 3			/* #  */


class WorkSpace : public QObject
{
	Q_OBJECT

	friend class MessageHandler;

private:

	QSharedPointer<Document> doc;
	QSharedPointer<QThread> workThread;
	QMap<QSslSocket*, QSharedPointer<Client>> editors;

	QTimer timer;
	quint16 nFails;

	MessageHandler messageHandler;

	SocketBuffer socketBuffer;

public:

	WorkSpace(QSharedPointer<Document> d, QObject* parent = 0);
	~WorkSpace();

public slots:

	void newClient(QSharedPointer<Client> client);
	void clientDisconnection();
	void clientQuit(QSslSocket* clientSocket);
	void socketErr(QAbstractSocket::SocketError socketError);

	void readMessage();
	void dispatchMessage(MessageCapsule message, QSslSocket* sender);
	
	void documentSave();
	void documentInsertSymbol(Symbol& symbol);
	void documentDeleteSymbol(QVector<qint32> position);
	void documentEditSymbol(QVector<qint32> position, QTextCharFormat format);
	void documentEditBlock(TextBlockID blockId, QTextBlockFormat format);

	void handleAccountUpdate(QSslSocket* clientSocket, QString nickname, QImage icon, QString password);
	void answerAccountUpdate(QSharedPointer<Client> client, MessageCapsule msg);

signals: void requestAccountUpdate(QSharedPointer<Client> client, QString nickname, QImage icon, QString password);
signals: void returnClient(QSharedPointer<Client> client);
signals: void userDisconnected(QString username);
signals: void noEditors(URI documentURI);

};


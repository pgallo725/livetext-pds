#pragma once
#ifndef __WORK_SPACE__
#define __WORK_SPACE__

#include <QThread>
#include <QTimer>
#include <QSslSocket>

#include <Document.h>
#include "Client.h"
#include "MessageHandler.h"
#include "ServerException.h"

#define DOCUMENT_SAVE_TIMEOUT 30000		/* ms */
#define DOCUMENT_MAX_FAILS 3			/* #  */


class WorkSpace : public QObject
{
	Q_OBJECT

private:

	QSharedPointer<Document> doc;
	QSharedPointer<QThread> workThread;
	QMap<QSslSocket*, QSharedPointer<Client>> editors;

	QTimer timer;
	quint16 nFails;

	MessageHandler messageHandler;

public:

	WorkSpace(QSharedPointer<Document> d, QObject* parent = 0);
	~WorkSpace();

public slots:

	void newClient(QSharedPointer<Client> client);
	void clientDisconnection();
	void clientQuit(QSslSocket* clientSocket, bool isForced);
	void socketAbort(QSslSocket* clientSocket);
	void socketErr(QAbstractSocket::SocketError socketError);

	void readMessage();
	void dispatchMessage(MessageCapsule message, QSslSocket* sender);
	
	void documentSave();
	void documentInsertSymbols(QVector<Symbol> symbols, TextBlockID blockId, QTextBlockFormat blockFmt);
	void documentDeleteSymbols(QVector<Position> positions);
	void documentEditSymbols(QVector<Position> positions, QVector<QTextCharFormat> formats);
	void documentEditBlock(TextBlockID blockId, QTextBlockFormat format);
	void documentEditList(TextBlockID blockId, TextListID listId, QTextListFormat format);

	void handleAccountUpdate(QSslSocket* clientSocket, QString nickname, QImage icon, QString password);
	void answerAccountUpdate(QSharedPointer<Client> client, MessageCapsule msg);

signals:

	void requestAccountUpdate(QSharedPointer<Client> client, QString nickname, QImage icon, QString password);
	void returnClient(QSharedPointer<Client> client);
	void userDisconnected(QString username);
	void noEditors(URI documentURI);

};
#endif


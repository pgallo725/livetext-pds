#pragma once

#include <QObject>
#include <QTimer>

#include <Document.h>
#include "Client.h"
#include "MessageHandler.h"
#include "ServerException.h"


class TcpServer;

class WorkSpace : public QObject
{
	Q_OBJECT

	friend class MessageHandler;

private:

	QSharedPointer<Document> doc;
	QSharedPointer<QThread> workThread;
	QMap<QTcpSocket*, QSharedPointer<Client>> editors;
	QTimer timer;

	MessageHandler messageHandler;

public:

	WorkSpace(QSharedPointer<Document> d, QObject* parent = 0);
	~WorkSpace();

public slots:

	//void newSocket(qint64 handle);
	void newClient(QSharedPointer<Client> client);
	void clientDisconnection();
	void readMessage();
	
	void documentSave();
	void documentInsertSymbol(Symbol& symbol);
	void documentDeleteSymbol(QVector<qint32> position);

	void dispatchMessage(MessageCapsule message, QTcpSocket* sender);

	MessageCapsule updateAccount(QTcpSocket* clientSocket, QString nickname, QImage icon, QString password);
	void clientQuit(QTcpSocket* clientSocket);

signals: void noEditors(URI documentURI);
signals: void returnClient(QSharedPointer<Client> client);

};


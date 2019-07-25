#pragma once

#include <QObject>
#include <list>
#include <queue>
#include <mutex>

#include "Document.h"
#include "Client.h"
#include "Message.h"


class TcpServer;

class WorkSpace : public QObject
{
	Q_OBJECT
private:
	QSharedPointer<Document> doc;
	QSharedPointer<TcpServer> server;
	QMap<QTcpSocket *, QSharedPointer<Client>> editors;

	void handleMessage(QSharedPointer<Message> msg, QTcpSocket* socket);

public:
	WorkSpace(QSharedPointer<Document> d, QSharedPointer<TcpServer> server, QObject* parent = 0);
	~WorkSpace();

public slots:
	void newSocket(qint64 handle);
	void clientDisconnection();
	void readMessage();

signals:
	void notWorking(QString document);
signals:
	void deleteClient(qint64 handle);
};


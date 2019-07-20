#pragma once

#include <QObject>
#include <list>
#include <queue>
#include <mutex>
#include "Document.h"
#include "Client.h"
#include "Message.h"
#include "ServerException.h"

class WorkSpace : public QObject
{
	Q_OBJECT
private:
	QSharedPointer<Document> doc;
	QList<QSharedPointer<QTcpSocket>> editors;

	void handleMessage(QSharedPointer<Message> msg, QTcpSocket* socket);

public:
	WorkSpace(QSharedPointer<Document> d);
	~WorkSpace();

public slots:
	void newSocket(qint64 handle);
	void clientDisconnection();
	void readMessage();

signals:
	void notWorking();
};


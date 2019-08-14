#pragma once

#include <QObject>
#include <QTimer>

#include "Document.h"
#include "Client.h"
#include "Message.h"
#include "AccountMessage.h"
#include "TextEditMessage.h"
#include "ErrorMessage.h"
#include "MessageHandler.h"
#include "ServerException.h"


class TcpServer;

class WorkSpace : public QObject
{
	Q_OBJECT

	friend class MessageHandler;

private:

	QSharedPointer<Document> doc;
	QSharedPointer<TcpServer> server;
	QMap<QTcpSocket*, QSharedPointer<Client>> editors;
	QTimer timer;

	MessageHandler messageHandler;

	bool updateAccount(User* oldUser, User& newUser);
	void dispatchMessage(std::unique_ptr<Message>&& msg, QTcpSocket* sender);

public:

	WorkSpace(QSharedPointer<Document> d, QSharedPointer<TcpServer> server);
	~WorkSpace();

public slots:

	void newSocket(qint64 handle);
	void clientDisconnection();
	void readMessage();
	void saveDocument();

signals: void notWorking(QString document);
signals: void deleteClient(qint64 handle);

};


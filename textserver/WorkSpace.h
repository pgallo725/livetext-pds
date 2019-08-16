#pragma once

#include <QObject>
#include <QTimer>

#include "Document.h"
#include "Client.h"
#include "Message.h"
#include "AccountMessage.h"
#include "TextEditMessage.h"
#include "PresenceMessage.h"
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

public:

	WorkSpace(QSharedPointer<Document> d, QSharedPointer<TcpServer> server);
	~WorkSpace();

public slots:

	void newSocket(qint64 handle);
	void clientDisconnection();
	void readMessage();
	
	void documentSave();
	void documentInsertSymbol(Symbol& symbol);
	void documentDeleteSymbol(QVector<qint32> position);

	void dispatchMessage(MessageCapsule message, QTcpSocket* sender);

	MessageCapsule updateAccount(QTcpSocket* clientSocket, User& updatedUser);
	MessageCapsule logoutClient(QTcpSocket* clientSocket);

signals: void notWorking(QString document);
signals: void deleteClient(qint64 handle);

};


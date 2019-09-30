#pragma once

#include <QObject>
#include <QSslSocket>

#include <Message.h>
#include <User.h>
#include <Document.h>


class WorkSpace;
class TcpServer;

class MessageHandler : public QObject
{
	Q_OBJECT

private:

	enum OwnerType
	{
		Server,
		Workspace
	};

private:

	OwnerType _usecase;

public:

	MessageHandler(WorkSpace* w);
	MessageHandler(TcpServer* s);

	void process(MessageCapsule message, QSslSocket* sender);

	~MessageHandler();


signals: MessageCapsule loginRequest(QSslSocket* clientSocket, QString username);
signals: MessageCapsule loginUnlock(QSslSocket* clientSocket, QByteArray token);

signals: MessageCapsule accountCreate(QSslSocket* çlientSocket, QString username, QString nickname, QImage icon, QString password);
signals: MessageCapsule accountUpdate(QSslSocket* çlientSocket, QString nickname, QImage icon, QString password);

signals: MessageCapsule documentCreate(QSslSocket* çlientSocket, QString docName);
signals: MessageCapsule documentOpen(QSslSocket* çlientSocket, URI docUri);
signals: MessageCapsule documentRemove(QSslSocket* çlientSocket, URI docUri);

signals: void charInsert(Symbol& s);
signals: void charDelete(QVector<qint32> pos);
signals: void blockEdit(QPair<qint32, qint32> id, QTextBlockFormat fmt);
signals: void messageDispatch(MessageCapsule message, QSslSocket* sender);

signals: void documentClose(QSslSocket* clientSocket);
signals: void userLogout(QSslSocket* clientSocket);

};

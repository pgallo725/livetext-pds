#pragma once

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

	enum Context
	{
		Server,
		Workspace
	};

private:

	Context _useCase;

public:

	MessageHandler(WorkSpace* w);
	MessageHandler(TcpServer* s);

	void process(MessageCapsule message, QSslSocket* sender);

	~MessageHandler();

signals:

	MessageCapsule loginRequest(QSslSocket* clientSocket, QString username);
	MessageCapsule loginUnlock(QSslSocket* clientSocket, QByteArray token);

	MessageCapsule accountCreate(QSslSocket* çlientSocket, QString username, QString nickname, QImage icon, QString password);
	MessageCapsule accountUpdate(QSslSocket* çlientSocket, QString nickname, QImage icon, QString password);

	MessageCapsule documentCreate(QSslSocket* çlientSocket, QString docName);
	MessageCapsule documentOpen(QSslSocket* çlientSocket, URI docUri, bool docJustCreated = false);
	MessageCapsule documentRemove(QSslSocket* çlientSocket, URI docUri);

	void charsInsert(QVector<Symbol> syms, TextBlockID bId, QTextBlockFormat blkFmt);
	void charsDelete(QVector<Position> poss);
	void charsFormat(QVector<Position> pos, QVector<QTextCharFormat> fmts);
	void blockEdit(TextBlockID id, QTextBlockFormat fmt);
	void listEdit(TextBlockID blockId, TextListID listId, QTextListFormat fmt);
	void messageDispatch(MessageCapsule message, QSslSocket* sender);

	void documentClose(QSslSocket* clientSocket, bool forced = false);
	void userLogout(QSslSocket* clientSocket);

};

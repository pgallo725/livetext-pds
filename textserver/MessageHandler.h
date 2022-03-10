#pragma once
#ifndef __MESSAGE_HANDLER__
#define __MESSAGE_HANDLER__

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

	MessageCapsule accountCreate(QSslSocket* clientSocket, QString username, QString nickname, QImage icon, QString password);
	MessageCapsule accountUpdate(QSslSocket* clientSocket, QString nickname, QImage icon, QString password);

	MessageCapsule documentCreate(QSslSocket* clientSocket, QString docName);
	MessageCapsule documentOpen(QSslSocket* clientSocket, URI docUri, bool docJustCreated = false);
	MessageCapsule documentRemove(QSslSocket* clientSocket, URI docUri);

	void charsInsert(QVector<Symbol> syms, TextBlockID bId, QTextBlockFormat blkFmt);
	void charsDelete(QVector<Position> poss);
	void charsFormat(QVector<Position> pos, QVector<QTextCharFormat> fmts);
	void blockEdit(TextBlockID id, QTextBlockFormat fmt);
	void listEdit(TextBlockID blockId, TextListID listId, QTextListFormat fmt);
	void messageDispatch(MessageCapsule message, QSslSocket* sender);

	void documentClose(QSslSocket* clientSocket, bool forced = false);
	void userLogout(QSslSocket* clientSocket);

};

#endif

#pragma once

#include "Message.h"
#include "User.h"
#include "Symbol.h"
#include "Document.h"


class MessageFactory
{
public:

	static MessageCapsule Empty(MessageType type);

	static MessageCapsule LoginRequest(QString username);
	static MessageCapsule LoginChallenge(QByteArray salt, QByteArray nonce);
	static MessageCapsule LoginUnlock(QByteArray token);
	static MessageCapsule LoginGranted(User user);
	static MessageCapsule LoginError(QString error);

	static MessageCapsule AccountCreate(QString username, QString nickname, QImage icon, QString password);
	static MessageCapsule AccountUpdate(QString nickname, QImage icon, QString password);
	static MessageCapsule AccountConfirmed(User user);
	static MessageCapsule AccountError(QString error);

	static MessageCapsule Logout();

	static MessageCapsule DocumentCreate(QString docName);
	static MessageCapsule DocumentRemove(QString docURI);
	static MessageCapsule DocumentOpen(QString docURI);
	static MessageCapsule DocumentDismissed();
	static MessageCapsule DocumentReady(Document doc);
	static MessageCapsule DocumentClose();
	static MessageCapsule DocumentExit();
	static MessageCapsule DocumentError(QString error);

	static MessageCapsule CharsInsert(QVector<Symbol> symbols, bool isLast, TextBlockID bId, QTextBlockFormat blkFmt);
	static MessageCapsule CharsDelete(QVector<Position> positions);
	static MessageCapsule CharsFormat(QVector<Position> positions, QVector<QTextCharFormat> fmts);
	static MessageCapsule BlockEdit(TextBlockID blockId, QTextBlockFormat fmt);
	static MessageCapsule ListEdit(TextBlockID blockId, TextListID listId, QTextListFormat fmt);

	static MessageCapsule CursorMove(qint32 userId, qint32 newPosition);
	static MessageCapsule PresenceUpdate(qint32 userId, QString nickname, QImage icon);
	static MessageCapsule PresenceAdd(qint32 userId, QString nickname, QImage icon);
	static MessageCapsule PresenceRemove(qint32 userId);

	static MessageCapsule Failure(QString error);
};
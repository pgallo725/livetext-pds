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
	static MessageCapsule LoginChallenge(QString nonce);
	static MessageCapsule LoginUnlock(QString token);
	static MessageCapsule LoginGranted(User user);
	static MessageCapsule LoginError(QString error);

	static MessageCapsule AccountCreate(User newUser);
	static MessageCapsule AccountUpdate(User updatedUser);
	static MessageCapsule AccountConfirmed(qint32 userId);
	static MessageCapsule AccountError(QString error);

	static MessageCapsule LogoutRequest();
	static MessageCapsule LogoutConfirmed();
	static MessageCapsule LogoutError(QString error);

	static MessageCapsule DocumentCreate(QString docName);
	static MessageCapsule DocumentRemove(QString docURI);
	static MessageCapsule DocumentOpen(QString docURI);
	static MessageCapsule DocumentDismissed();
	static MessageCapsule DocumentReady(Document doc);
	static MessageCapsule DocumentError(QString error);

	static MessageCapsule CharInsert(Symbol symbol);
	static MessageCapsule CharDelete(QVector<qint32> position);

	static MessageCapsule CursorMove(qint32 userId, qint32 newPosition);
	static MessageCapsule PresenceUpdate(qint32 userId, QString nickname, QImage icon);
	static MessageCapsule PresenceAdd(qint32 userId, QString nickname, QImage icon);
	static MessageCapsule PresenceRemove(qint32 userId);

	static MessageCapsule Failure(QString error);
};
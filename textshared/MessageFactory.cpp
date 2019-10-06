#include "MessageFactory.h"

#include "LoginMessage.h"
#include "AccountMessage.h"
#include "LogoutMessage.h"
#include "DocumentMessage.h"
#include "TextEditMessage.h"
#include "PresenceMessage.h"
#include "FailureMessage.h"
#include <SharedException.h>


MessageCapsule MessageFactory::Empty(MessageType type)
{
	switch (type)
	{
		case MessageType::LoginRequest:			return new LoginRequestMessage();
		case MessageType::LoginChallenge:		return new LoginChallengeMessage();
		case MessageType::LoginUnlock:			return new LoginUnlockMessage();
		case MessageType::LoginGranted:			return new LoginGrantedMessage();
		case MessageType::LoginError:			return new LoginErrorMessage();
		case MessageType::AccountCreate:		return new AccountCreateMessage();
		case MessageType::AccountUpdate:		return new AccountUpdateMessage();
		case MessageType::AccountConfirmed:		return new AccountConfirmedMessage();
		case MessageType::AccountError:			return new AccountErrorMessage();
		case MessageType::Logout:				return new LogoutMessage();
		case MessageType::DocumentCreate:		return new DocumentCreateMessage();
		case MessageType::DocumentRemove:		return new DocumentRemoveMessage();
		case MessageType::DocumentDismissed:	return new DocumentDismissedMessage();
		case MessageType::DocumentOpen:			return new DocumentOpenMessage();
		case MessageType::DocumentReady:		return new DocumentReadyMessage();
		case MessageType::DocumentClose:		return new DocumentCloseMessage();
		case MessageType::DocumentExit:			return new DocumentExitMessage();
		case MessageType::DocumentError:		return new DocumentErrorMessage();
		case MessageType::CharInsert:			return new CharInsertMessage();
		case MessageType::CharDelete:			return new CharDeleteMessage();
		case MessageType::BlockEdit:			return new BlockEditMessage();
		case MessageType::CursorMove:			return new CursorMoveMessage();
		case MessageType::PresenceUpdate:		return new PresenceUpdateMessage();
		case MessageType::PresenceAdd:			return new PresenceAddMessage();
		case MessageType::PresenceRemove:		return new PresenceRemoveMessage();
		case MessageType::Failure:				return new FailureMessage();

		default:
			throw MessageTypeException("Type unknown", type);
			break;
	}
}


MessageCapsule MessageFactory::LoginRequest(QString username)
{
	return new LoginRequestMessage(username);
}

MessageCapsule MessageFactory::LoginChallenge(QByteArray salt, QByteArray nonce)
{
	return new LoginChallengeMessage(salt, nonce);
}

MessageCapsule MessageFactory::LoginUnlock(QByteArray token)
{
	return new LoginUnlockMessage(token);
}

MessageCapsule MessageFactory::LoginGranted(User user)
{
	return new LoginGrantedMessage(user);
}

MessageCapsule MessageFactory::LoginError(QString error)
{
	return new LoginErrorMessage(error);
}

MessageCapsule MessageFactory::AccountCreate(QString username, QString nickname, QImage icon, QString password)
{
	return new AccountCreateMessage(username, nickname, icon, password);
}

MessageCapsule MessageFactory::AccountUpdate(QString nickname, QImage icon, QString password)
{
	return new AccountUpdateMessage(nickname, icon, password);
}

MessageCapsule MessageFactory::AccountConfirmed(User user)
{
	return new AccountConfirmedMessage(user);
}

MessageCapsule MessageFactory::AccountError(QString error)
{
	return new AccountErrorMessage(error);
}

MessageCapsule MessageFactory::Logout()
{
	return new LogoutMessage();
}

MessageCapsule MessageFactory::DocumentCreate(QString docName)
{
	return new DocumentCreateMessage(docName);
}

MessageCapsule MessageFactory::DocumentRemove(QString docURI)
{
	return new DocumentRemoveMessage(docURI);
}

MessageCapsule MessageFactory::DocumentOpen(QString docURI)
{
	return new DocumentOpenMessage(docURI);
}

MessageCapsule MessageFactory::DocumentDismissed()
{
	return new DocumentDismissedMessage();
}

MessageCapsule MessageFactory::DocumentReady(Document doc)
{
	return new DocumentReadyMessage(doc);
}

MessageCapsule MessageFactory::DocumentClose()
{
	return new DocumentCloseMessage();
}

MessageCapsule MessageFactory::DocumentExit()
{
	return new DocumentExitMessage();
}

MessageCapsule MessageFactory::DocumentError(QString error)
{
	return new DocumentErrorMessage(error);
}

MessageCapsule MessageFactory::CharInsert(Symbol symbol)
{
	return new CharInsertMessage(symbol);
}

MessageCapsule MessageFactory::CharDelete(QVector<qint32> position)
{
	return new CharDeleteMessage(position);
}

MessageCapsule MessageFactory::BlockEdit(TextBlockID blockId, QTextBlockFormat fmt, qint32 editorId)
{
	return new BlockEditMessage(blockId, fmt, editorId);
}

MessageCapsule MessageFactory::CursorMove(qint32 userId, qint32 newPosition)
{
	return new CursorMoveMessage(userId, newPosition);
}

MessageCapsule MessageFactory::PresenceUpdate(qint32 userId, QString nickname, QImage icon)
{
	return new PresenceUpdateMessage(userId, nickname, icon);
}

MessageCapsule MessageFactory::PresenceAdd(qint32 userId, QString nickname, QImage icon)
{
	return new PresenceAddMessage(userId, nickname, icon);
}

MessageCapsule MessageFactory::PresenceRemove(qint32 userId)
{
	return new PresenceRemoveMessage(userId);
}

MessageCapsule MessageFactory::Failure(QString error)
{
	return new FailureMessage(error);
}

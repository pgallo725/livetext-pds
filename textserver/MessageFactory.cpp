#include "MessageFactory.h"

#include "LoginMessage.h"
#include "AccountMessage.h"
#include "LogoutMessage.h"
#include "DocumentMessage.h"
#include "TextEditMessage.h"
#include "PresenceMessage.h"
#include "FailureMessage.h"



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
		case MessageType::LogoutRequest:		return new LogoutRequestMessage();
		case MessageType::LogoutConfirmed:		return new LogoutConfirmedMessage();
		case MessageType::LogoutError:			return new LogoutErrorMessage();
		case MessageType::DocumentCreate:		return new DocumentCreateMessage();
		case MessageType::DocumentRemove:		return new DocumentRemoveMessage();
		case MessageType::DocumentDismissed:	return new DocumentDismissedMessage();
		case MessageType::DocumentOpen:			return new DocumentOpenMessage();
		case MessageType::DocumentReady:		return new DocumentReadyMessage();
		case MessageType::DocumentError:		return new DocumentErrorMessage();
		case MessageType::CharInsert:			return new CharInsertMessage();
		case MessageType::CharDelete:			return new CharDeleteMessage();
		case MessageType::CursorMove:			return new CursorMoveMessage();
		case MessageType::PresenceUpdate:		return new PresenceUpdateMessage();
		case MessageType::PresenceAdd:			return new PresenceAddMessage();
		case MessageType::PresenceRemove:		return new PresenceRemoveMessage();
		case MessageType::Failure:				return new FailureMessage();

		default:
			throw 1;
			break;
	}
}


MessageCapsule MessageFactory::LoginRequest(QString username)
{
	return new LoginRequestMessage(username);
}

MessageCapsule MessageFactory::LoginChallenge(QString nonce)
{
	return new LoginChallengeMessage(nonce);
}

MessageCapsule MessageFactory::LoginUnlock(QString token)
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

MessageCapsule MessageFactory::AccountCreate(User newUser)
{
	return new AccountCreateMessage(newUser);
}

MessageCapsule MessageFactory::AccountUpdate(User updatedUser)
{
	return new AccountUpdateMessage(updatedUser);
}

MessageCapsule MessageFactory::AccountConfirmed(qint32 userId)
{
	return new AccountConfirmedMessage(userId);
}

MessageCapsule MessageFactory::AccountError(QString error)
{
	return new AccountErrorMessage(error);
}

MessageCapsule MessageFactory::LogoutRequest()
{
	return new LogoutRequestMessage();
}

MessageCapsule MessageFactory::LogoutConfirmed()
{
	return new LogoutConfirmedMessage();
}

MessageCapsule MessageFactory::LogoutError(QString error)
{
	return new LogoutErrorMessage(error);
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

#pragma once

#include <QObject>


enum MessageType
{
	LoginRequest,
	LoginChallenge,
	LoginUnlock,
	LoginAccessGranted,
	LoginError,
	AccountCreate,
	AccountUpDate,
	AccountConfirmed,
	AccountDenied,
	LogoutRequest,
	LogoutConfirmed,
	LogoutDenied,
	NewDocument,
	OpenDocument,
	UriRequest,
	UriResponse,
	DocumentOpened,
	DocumentRemove,
	DocumentError,
	CharInsert,
	CharDelete,
	MoveCursor,
	UserNameChange,
	UserIconChange,
	AddUserPresence,
	RemoveUserPresence,
	WrongMessageType
};

class Message : public QObject
{
	Q_OBJECT

protected:

	MessageType m_type;

public:

	Message(MessageType type) : m_type(type) { };
	~Message() { };

	/* getter */
	int getType() {
		return m_type;
	};
};

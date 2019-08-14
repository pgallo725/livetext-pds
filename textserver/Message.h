#pragma once

#include <QObject>
#include <QTcpSocket>
#include <QDataStream>


enum MessageType
{
	// Login messages
	LoginRequest,
	LoginChallenge,
	LoginUnlock,
	LoginAccessGranted,
	LoginError,

	// Account messages
	AccountCreate,
	AccountUpdate,
	AccountConfirmed,
	AccountDenied,

	// Logout messages
	LogoutRequest,
	LogoutConfirmed,
	LogoutDenied,

	// Document messages
	NewDocument,
	RemoveDocument,
	OpenDocument,
	DocumentOpened,
	DocumentError,

	// Text-editing messages
	CharInsert,
	CharDelete,

	// Presence messages
	MoveCursor,
	UserNameChange,
	UserIconChange,
	AddUserPresence,
	RemoveUserPresence,

	// Others
	MessageTypeError
};


class Message : public QObject
{
	Q_OBJECT

protected:

	MessageType m_type;

public:

	Message(MessageType type) : m_type(type) { };
	~Message() { };

	// Convert the message contents into a byte-stream that will be sent on the socket
	virtual void sendTo(QTcpSocket* socket) = 0;

	// Read from the data-stream the bytes needed to fill this message, according to its type
	virtual void readFrom(QDataStream& stream) = 0;

	/* getter */
	int getType() { return m_type; };
};

#pragma once

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
	DocumentRemoved,
	OpenDocument,
	DocumentOpened,
	DocumentError,

	// Text-editing messages
	CharInsert,
	CharDelete,

	// Presence messages
	MoveCursor,
	UserAccountUpdate,
	AddUserPresence,
	RemoveUserPresence,

	// Others
	MessageTypeError,
	ServerFailure
};


class Message
{

protected:

	MessageType m_type;

public:

	Message(MessageType type);
	~Message() { };

	// Convert the message contents into a byte-stream that will be sent on the socket
	virtual void sendTo(QTcpSocket* socket) = 0;

	// Read from the data-stream the bytes needed to fill this message, according to its type
	virtual void readFrom(QDataStream& stream) = 0;

	/* getter */
	int getType();
};


class MessageCapsule
{
private:

	Message* m_ptr;
	int* ref;

public:

	MessageCapsule();
	MessageCapsule(std::nullptr_t);
	MessageCapsule(Message* m);

	MessageCapsule(const MessageCapsule& other);
	MessageCapsule(MessageCapsule&& other);

	MessageCapsule& operator=(MessageCapsule other);

	Message& operator*() const;
	Message* operator->() const;

	operator bool() const;

	Message* get() const;
	void reset();

	~MessageCapsule();
};
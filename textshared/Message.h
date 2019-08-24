#pragma once

#include <QTcpSocket>
#include <QDataStream>


enum MessageType
{
	// Login messages
	LoginRequest,
	LoginChallenge,
	LoginUnlock,
	LoginGranted,
	LoginError,

	// Account messages
	AccountCreate,
	AccountUpdate,
	AccountConfirmed,
	AccountError,

	// Logout messages
	LogoutRequest,
	LogoutConfirmed,
	LogoutError,

	// Document messages
	DocumentCreate,
	DocumentRemove,
	DocumentDismissed,
	DocumentOpen,
	DocumentReady,
	DocumentError,

	// Text-editing messages
	CharInsert,
	CharDelete,

	// Presence messages
	CursorMove,
	PresenceUpdate,
	PresenceAdd,
	PresenceRemove,

	// Others
	Failure
};


class MessageFactory;


class Message
{

protected:

	MessageType m_type;

public:

	Message(MessageType type);

	virtual ~Message() { };

	// Convert the message contents into a byte-stream that will be sent on the socket
	virtual void sendTo(QTcpSocket* socket) const = 0;

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
	MessageCapsule(MessageCapsule&& other) noexcept;

	MessageCapsule& operator=(MessageCapsule other);

	Message& operator*() const;
	Message* operator->() const;

	operator bool() const;

	Message* get() const;
	void reset();

	~MessageCapsule();
};
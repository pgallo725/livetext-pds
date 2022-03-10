#pragma once
#ifndef _MESSAGE_
#define _MESSAGE_

#include <QSslSocket>
#include <QDataStream>
#include <QByteArray>

enum MessageType : quint16
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

	// Logout message
	Logout,

	// Document messages
	DocumentCreate,
	DocumentRemove,
	DocumentDismissed,
	DocumentOpen,
	DocumentReady,
	DocumentClose,
	DocumentExit,
	DocumentError,

	// Text-editing messages
	CharsInsert,
	CharsDelete,
	CharsFormat,
	BlockEdit,
	ListEdit,

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

	// Utility method for translating MessageType codes into human-readable names
	static std::string TypeName(MessageType type);

	// Handles serialization of the message contents to a byte-stream and writes it on the socket
	void send(QSslSocket* socket) const;

	// Call readFrom and check stream status
	void read(QDataStream& stream);
	
	/* getter */
	MessageType getType();

protected:

	// Write the message fields on the data-stream's buffer
	virtual void writeTo(QDataStream& stream) const = 0;

	// Read from the data-stream the bytes needed to fill this message, according to its type
	virtual void readFrom(QDataStream& stream) = 0;
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

#endif // _MESSAGE_

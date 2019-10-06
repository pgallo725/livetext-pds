#include "Message.h"
#include <SharedException.h>

Message::Message(MessageType type)
	: m_type(type)
{
}

void Message::send(QSslSocket* socket) const
{
	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);

	// Serialize the message contents on the byte-stream, first the Type and Length fields,
	// then the Payload (specific to each message) and therefore handled by the sub-class
	stream << m_type << quint32(0); 
	this->writeTo(stream);

	// Seek back to the beginning of the message byte-stream, right after 
	// the Type field, and write the proper value in the Length field
	stream.device()->seek(sizeof(MessageType));
	stream << (quint32)(buffer.size() - sizeof(MessageType) - sizeof(quint32));

	if (stream.status() == QDataStream::WriteFailed) {
		throw MessageWriteException("Cannot write message on stream", m_type);
	}

	// Write the buffer on the socket and send it immediately
	if (socket->write(buffer) < 0) {
		throw MessageWriteException("Cannot write message on socket", m_type);
	}
	socket->flush();
}

void Message::read(QDataStream& stream)
{
	readFrom(stream);

	if (stream.status() != QDataStream::Ok || !stream.atEnd()) {
		throw MessageReadException("Read corrupted streamData", m_type);
	}
}

int Message::getType()
{
	return m_type;
}



/******************** MESSAGE CAPSULE METHODS ********************/


MessageCapsule::MessageCapsule()
	: m_ptr(nullptr), ref(nullptr)
{
}

MessageCapsule::MessageCapsule(std::nullptr_t)
	: m_ptr(nullptr), ref(nullptr)
{
}

MessageCapsule::MessageCapsule(Message* m)
try
	: m_ptr(m), ref(new int(1))
{
}
catch (...)
{
	delete m;
	throw;
}

MessageCapsule::MessageCapsule(const MessageCapsule& other)
	: m_ptr(other.m_ptr), ref(other.ref)
{
	if (m_ptr != nullptr)
		++(*ref);
}

MessageCapsule::MessageCapsule(MessageCapsule&& other) noexcept
	: m_ptr(other.m_ptr), ref(other.ref)
{
	other.m_ptr = nullptr;
}

MessageCapsule& MessageCapsule::operator=(MessageCapsule other)
{
	// Copy & Swap assignment operator implementation
	std::swap(this->m_ptr, other.m_ptr);
	std::swap(this->ref, other.ref);
	return *this;
}

Message& MessageCapsule::operator*() const
{
	return *m_ptr;
}

Message* MessageCapsule::operator->() const
{
	return m_ptr;
}

MessageCapsule::operator bool() const
{
	return m_ptr != nullptr;
}

Message* MessageCapsule::get() const
{
	return m_ptr;
}

void MessageCapsule::reset()
{
	if (m_ptr != nullptr)
	{
		--(*ref);
		if (*ref == 0)
		{
			delete m_ptr;
			delete ref;
		}	
	}
	
	m_ptr = nullptr;
	ref = nullptr;
}

MessageCapsule::~MessageCapsule()
{
	if (m_ptr != nullptr)
	{
		--(*ref);
		if (*ref == 0)
		{
			delete m_ptr;
			delete ref;
		}
	}
}

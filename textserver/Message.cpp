#include "Message.h"


Message::Message(MessageType type)
	: m_type(type)
{
}

int Message::getType()
{
	return m_type;
}



/******************** MESSAGE CAPSULE METHODS ********************/


MessageCapsule::MessageCapsule()
	: m_ptr(nullptr), ref(new int(0))
{
}

MessageCapsule::MessageCapsule(std::nullptr_t)
	: m_ptr(nullptr), ref(new int(0))
{
}

MessageCapsule::MessageCapsule(Message* m)
try
	: ref(new int(1)), m_ptr(m)
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
	++(*ref);
}

MessageCapsule::MessageCapsule(MessageCapsule&& other)
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
	--(*ref);
	if (*ref == 0)
	{
		if (m_ptr != nullptr)
			delete m_ptr;
	}
	m_ptr = nullptr;
}

MessageCapsule::~MessageCapsule()
{
	if (m_ptr != nullptr)
	{
		--(*ref);
		if (*ref == 0)
		{
			delete m_ptr;
		}
	}
}

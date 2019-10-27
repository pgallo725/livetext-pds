#include "FailureMessage.h"


FailureMessage::FailureMessage()
	: Message(Failure)
{
}

FailureMessage::FailureMessage(QString description)
	: Message(Failure), m_error(description)
{
}

void FailureMessage::writeTo(QDataStream& stream) const
{
	stream << m_error;
}

void FailureMessage::readFrom(QDataStream& stream)
{
	stream >> m_error;
}

QString FailureMessage::getDescription() const
{
	return m_error;
}

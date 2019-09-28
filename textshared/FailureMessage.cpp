#include "FailureMessage.h"


FailureMessage::FailureMessage()
	: Message(Failure)
{
}

FailureMessage::FailureMessage(QString description)
	: Message(Failure), m_error(description)
{
}

void FailureMessage::writeContent(QDataStream& stream) const
{
	stream << m_error;
}

void FailureMessage::readFrom(QDataStream& stream)
{
	stream >> m_error;
}

QString FailureMessage::getDescription()
{
	return m_error;
}

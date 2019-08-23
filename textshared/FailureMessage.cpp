#include "FailureMessage.h"


FailureMessage::FailureMessage()
	: Message(Failure)
{
}

FailureMessage::FailureMessage(QString description)
	: Message(Failure), m_error(description)
{
}

void FailureMessage::readFrom(QDataStream& stream)
{
	stream >> m_error;
}

void FailureMessage::sendTo(QTcpSocket* socket) const
{
	QDataStream streamOut(socket);

	streamOut << (quint16)m_type << m_error;
}

QString FailureMessage::getDescription()
{
	return m_error;
}

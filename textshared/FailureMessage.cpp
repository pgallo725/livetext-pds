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

void FailureMessage::sendTo(QSslSocket* socket) const
{
	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);

	stream << Failure << quint32(0) << m_error;

	stream.device()->seek(sizeof(MessageType));
	stream << (quint32)(buffer.size() - sizeof(MessageType) - sizeof(quint32));
	socket->write(buffer);
	socket->flush();
}

QString FailureMessage::getDescription()
{
	return m_error;
}

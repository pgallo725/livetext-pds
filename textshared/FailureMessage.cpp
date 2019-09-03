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
	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);

	stream << Failure << qint32(0) << m_error;

	stream.device()->seek(sizeof(MessageType));
	stream << (qint32)buffer.size() - sizeof(MessageType) - sizeof(qint32);
	socket->write(buffer);
	socket->flush();
}

QString FailureMessage::getDescription()
{
	return m_error;
}

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
	QByteArray dataBuffer;
	QDataStream streamBuffer(&dataBuffer, QIODevice::WriteOnly);

	streamBuffer << quint16(0) << qint32(0) << m_error;

	streamBuffer.device()->seek(0);
	streamBuffer << (quint16)m_type
		<< (qint32)dataBuffer.size() - sizeof(qint32) - sizeof(quint16);
	socket->write(dataBuffer);
	socket->flush();

	//streamOut << (quint16)m_type << bufferData.size() << bufferData;

	//streamOut << (quint16)m_type << m_error;
}

QString FailureMessage::getDescription()
{
	return m_error;
}

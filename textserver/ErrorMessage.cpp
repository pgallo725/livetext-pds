#include "ErrorMessage.h"


ErrorMessage::ErrorMessage(MessageType m)
	: Message(m)
{
}

ErrorMessage::ErrorMessage(MessageType messageError, QString description)
	: Message(messageError), m_error(description)
{
}


void ErrorMessage::readFrom(QDataStream& stream)
{
	stream >> m_error;
}

void ErrorMessage::sendTo(QTcpSocket* socket)
{
	QDataStream streamOut(socket);

	streamOut << (quint16)m_type << m_error;
}


QString ErrorMessage::getDescription()
{
	return m_error;
}

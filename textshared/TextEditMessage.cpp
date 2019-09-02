#include "TextEditMessage.h"


/*************** CHAR INSERT MESSAGE ***************/

CharInsertMessage::CharInsertMessage()
	: Message(CharInsert)
{
}

CharInsertMessage::CharInsertMessage(Symbol symbol)
	: Message(CharInsert), m_symbol(symbol)
{
}

void CharInsertMessage::readFrom(QDataStream& stream)
{
	stream >> m_symbol;
}

void CharInsertMessage::sendTo(QTcpSocket* socket) const
{
	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);

	stream << CharInsert << qint32(0) << m_symbol;

	stream.device()->seek(sizeof(MessageType));
	stream << (qint32)buffer.size() - sizeof(qint32);
	socket->write(buffer);
	socket->flush();
}

Symbol& CharInsertMessage::getSymbol()
{
	return m_symbol;
}


/*************** CHAR DELETE MESSAGE ***************/

CharDeleteMessage::CharDeleteMessage()
	: Message(CharDelete)
{
}

CharDeleteMessage::CharDeleteMessage(QVector<qint32> position)
	: Message(CharDelete), m_fPos(position)
{
}

void CharDeleteMessage::readFrom(QDataStream& stream)
{
	stream >> m_fPos;
}

void CharDeleteMessage::sendTo(QTcpSocket* socket) const
{
	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);

	stream << CharDelete << qint32(0) << m_fPos;

	stream.device()->seek(sizeof(MessageType));
	stream << (qint32)buffer.size() - sizeof(qint32);
	socket->write(buffer);
	socket->flush();
}

QVector<qint32> CharDeleteMessage::getPosition() const
{
	return m_fPos;
}

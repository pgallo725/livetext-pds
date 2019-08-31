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
	QDataStream streamOut(socket);
	QBuffer bufferData;
	QDataStream streamBuffer(&bufferData);

	streamBuffer << m_symbol;

	streamOut << (quint16)CharInsert << bufferData.size() << bufferData.data();
	//streamOut << (quint16)CharInsert << m_symbol;
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
	QDataStream streamOut(socket);
	QBuffer bufferData;
	QDataStream streamBuffer(&bufferData);

	streamBuffer << m_fPos;

	streamOut << (quint16)CharDelete << bufferData.size() << bufferData.data();
	//streamOut << (quint16)CharDelete << m_fPos;
}

QVector<qint32> CharDeleteMessage::getPosition() const
{
	return m_fPos;
}

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

void CharInsertMessage::sendTo(QSslSocket* socket) const
{
	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);

	stream << CharInsert << quint32(0) << m_symbol;

	stream.device()->seek(sizeof(MessageType));
	stream << (quint32)(buffer.size() - sizeof(MessageType) - sizeof(quint32));
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

void CharDeleteMessage::sendTo(QSslSocket* socket) const
{
	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);

	stream << CharDelete << quint32(0) << m_fPos;

	stream.device()->seek(sizeof(MessageType));
	stream << (quint32)(buffer.size() - sizeof(MessageType) - sizeof(quint32));
	socket->write(buffer);
	socket->flush();
}

QVector<qint32> CharDeleteMessage::getPosition() const
{
	return m_fPos;
}


/*************** BLOCK FORMAT EDIT MESSAGE ***************/

BlockEditMessage::BlockEditMessage()
	: Message(BlockEdit), m_editorId(-1)
{
}

BlockEditMessage::BlockEditMessage(QPair<qint32, qint32> blockId, QTextBlockFormat fmt, qint32 editorId)
	: Message(BlockEdit), m_blockId(blockId), m_blockFmt(fmt), m_editorId(editorId)
{
}

void BlockEditMessage::readFrom(QDataStream& stream)
{
	stream >> m_blockId >> m_blockFmt >> m_editorId;
}

void BlockEditMessage::sendTo(QSslSocket* socket) const
{
	QByteArray buffer;
	QDataStream stream(&buffer, QIODevice::WriteOnly);

	stream << BlockEdit << quint32(0) << m_blockId << m_blockFmt << m_editorId;

	stream.device()->seek(sizeof(MessageType));
	stream << (quint32)(buffer.size() - sizeof(MessageType) - sizeof(quint32));
	socket->write(buffer);
	socket->flush();
}

qint32 BlockEditMessage::getAuthorId() const
{
	return m_editorId;
}

QPair<qint32, qint32> BlockEditMessage::getBlockIdPair() const
{
	return m_blockId;
}

QTextBlockFormat BlockEditMessage::getBlockFormat() const
{
	return m_blockFmt;
}

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

void CharInsertMessage::writeTo(QDataStream& stream) const
{
	stream << m_symbol;
}

void CharInsertMessage::readFrom(QDataStream& stream)
{
	stream >> m_symbol;
}

Symbol& CharInsertMessage::getSymbol()
{
	return m_symbol;
}


/*************** CHAR FORMAT MESSAGE ***************/

CharFormatMessage::CharFormatMessage()
	: Message(CharDelete)
{
}

CharFormatMessage::CharFormatMessage(QVector<qint32> position, QTextCharFormat fmt)
	: Message(CharDelete), m_fPos(position), m_fmt(fmt)
{
}

void CharFormatMessage::writeTo(QDataStream& stream) const
{
	stream << m_fPos << m_fmt;
}

void CharFormatMessage::readFrom(QDataStream& stream)
{
	stream >> m_fPos >> m_fmt;
}

QVector<qint32> CharFormatMessage::getPosition() const
{
	return m_fPos;
}

QTextCharFormat CharFormatMessage::getCharFormat() const
{
	return m_fmt;
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

void CharDeleteMessage::writeTo(QDataStream& stream) const
{
	stream << m_fPos;
}

void CharDeleteMessage::readFrom(QDataStream& stream)
{
	stream >> m_fPos;
}

QVector<qint32> CharDeleteMessage::getPosition() const
{
	return m_fPos;
}


/*************** BLOCK FORMAT EDIT MESSAGE ***************/

BlockEditMessage::BlockEditMessage()
	: Message(BlockEdit), m_blockId(TextBlockID(nullptr)), m_editorId(-1)
{
}

BlockEditMessage::BlockEditMessage(TextBlockID blockId, QTextBlockFormat fmt, qint32 editorId)
	: Message(BlockEdit), m_blockId(blockId), m_blockFmt(fmt), m_editorId(editorId)
{
}

void BlockEditMessage::writeTo(QDataStream& stream) const
{
	stream << m_blockId << m_blockFmt << m_editorId;
}

void BlockEditMessage::readFrom(QDataStream& stream)
{
	stream >> m_blockId >> m_blockFmt >> m_editorId;
}

qint32 BlockEditMessage::getAuthorId() const
{
	return m_editorId;
}

TextBlockID BlockEditMessage::getBlockId() const
{
	return m_blockId;
}

QTextBlockFormat BlockEditMessage::getBlockFormat() const
{
	return m_blockFmt;
}

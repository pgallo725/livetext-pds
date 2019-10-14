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


/*************** CHAR FORMAT MESSAGE ***************/

CharFormatMessage::CharFormatMessage()
	: Message(CharFormat)
{
}

CharFormatMessage::CharFormatMessage(QVector<qint32> position, QTextCharFormat fmt)
	: Message(CharFormat), m_fPos(position), m_charFmt(fmt)
{
}

void CharFormatMessage::writeTo(QDataStream& stream) const
{
	stream << m_fPos << m_charFmt;
}

void CharFormatMessage::readFrom(QDataStream& stream)
{
	stream >> m_fPos >> m_charFmt;
}

QVector<qint32> CharFormatMessage::getPosition() const
{
	return m_fPos;
}

QTextCharFormat CharFormatMessage::getCharFormat() const
{
	return m_charFmt;
}


/*************** BLOCK FORMAT EDIT MESSAGE ***************/

BlockEditMessage::BlockEditMessage()
	: Message(BlockEdit), m_blockId(TextBlockID(nullptr))
{
}

BlockEditMessage::BlockEditMessage(TextBlockID blockId, QTextBlockFormat fmt)
	: Message(BlockEdit), m_blockId(blockId), m_blockFmt(fmt)
{
}

void BlockEditMessage::writeTo(QDataStream& stream) const
{
	stream << m_blockId << m_blockFmt;
}

void BlockEditMessage::readFrom(QDataStream& stream)
{
	stream >> m_blockId >> m_blockFmt;
}

TextBlockID BlockEditMessage::getBlockId() const
{
	return m_blockId;
}

QTextBlockFormat BlockEditMessage::getBlockFormat() const
{
	return m_blockFmt;
}

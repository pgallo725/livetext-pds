#include "TextEditMessage.h"


/*************** CHARS INSERT MESSAGE ***************/

CharsInsertMessage::CharsInsertMessage()
	: Message(CharsInsert), m_flag(false)
{
}

CharsInsertMessage::CharsInsertMessage(QVector<Symbol> symbols, bool isLast, TextBlockID bId, QTextBlockFormat blkFmt)
	: Message(CharsInsert), m_symbols(symbols), m_blockId(bId), m_blockFmt(blkFmt), m_flag(isLast)
{
	m_symbols.squeeze();	// Avoid any unrequired memory usage to reduce message size
}

void CharsInsertMessage::writeTo(QDataStream& stream) const
{
	stream << m_symbols << m_blockId << m_blockFmt << m_flag;
}

void CharsInsertMessage::readFrom(QDataStream& stream)
{
	stream >> m_symbols >> m_blockId >> m_blockFmt >> m_flag;
}

QVector<Symbol> CharsInsertMessage::getSymbols() const
{
	return m_symbols;
}

TextBlockID CharsInsertMessage::getBlockId() const
{
	return m_blockId;
}

QTextBlockFormat CharsInsertMessage::getBlockFormat() const
{
	return m_blockFmt;
}

bool CharsInsertMessage::getIsLast() const
{
	return m_flag;
}


/*************** CHARS DELETE MESSAGE ***************/

CharsDeleteMessage::CharsDeleteMessage()
	: Message(CharsDelete)
{
}

CharsDeleteMessage::CharsDeleteMessage(QVector<Position> positions)
	: Message(CharsDelete), m_fPositions(positions)
{
	m_fPositions.squeeze();		// Avoid any extra memory usage to reduce message size
}

void CharsDeleteMessage::writeTo(QDataStream& stream) const
{
	stream << m_fPositions;
}

void CharsDeleteMessage::readFrom(QDataStream& stream)
{
	stream >> m_fPositions;
}

QVector<Position> CharsDeleteMessage::getPositions() const
{
	return m_fPositions;
}


/*************** CHAR FORMAT MESSAGE ***************/

CharsFormatMessage::CharsFormatMessage()
	: Message(CharsFormat)
{
}

CharsFormatMessage::CharsFormatMessage(QVector<Position> positions, QVector<QTextCharFormat> fmts)
	: Message(CharsFormat), m_fPos(positions), m_charFmt(fmts)
{
}

void CharsFormatMessage::writeTo(QDataStream& stream) const
{
	stream << m_fPos << m_charFmt;
}

void CharsFormatMessage::readFrom(QDataStream& stream)
{
	stream >> m_fPos >> m_charFmt;
}

QVector<Position> CharsFormatMessage::getPositions() const
{
	return m_fPos;
}

QVector<QTextCharFormat> CharsFormatMessage::getCharFormats() const
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


/*************** LIST BLOCKS EDIT MESSAGE ***************/

ListEditMessage::ListEditMessage()
	: Message(ListEdit), m_blockId(TextBlockID(nullptr)), m_listId(0)
{
}

ListEditMessage::ListEditMessage(TextBlockID blockId, TextListID listId, QTextListFormat fmt)
	: Message(ListEdit), m_blockId(blockId), m_listId(listId), m_listFmt(fmt)
{
}

void ListEditMessage::writeTo(QDataStream& stream) const
{
	stream << m_blockId << m_listId << m_listFmt;
}

void ListEditMessage::readFrom(QDataStream& stream)
{
	stream >> m_blockId >> m_listId >> m_listFmt;
}

TextBlockID ListEditMessage::getBlockId() const
{
	return m_blockId;
}

TextListID ListEditMessage::getListId() const
{
	return m_listId;
}

QTextListFormat ListEditMessage::getListFormat() const
{
	return m_listFmt;
}

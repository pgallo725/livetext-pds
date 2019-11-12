#include "TextEditMessage.h"


/*************** CHAR INSERT MESSAGE ***************/

CharInsertMessage::CharInsertMessage()
	: Message(CharInsert), m_flag(false)
{
}

CharInsertMessage::CharInsertMessage(Symbol symbol, bool isLast)
	: Message(CharInsert), m_symbol(symbol), m_flag(isLast)
{
}

void CharInsertMessage::writeTo(QDataStream& stream) const
{
	stream << m_symbol << m_flag;
}

void CharInsertMessage::readFrom(QDataStream& stream)
{
	stream >> m_symbol >> m_flag;
}

Symbol CharInsertMessage::getSymbol() const
{
	return m_symbol;
}

bool CharInsertMessage::getIsLast() const
{
	return m_flag;
}


/*************** CHAR DELETE MESSAGE ***************/

CharDeleteMessage::CharDeleteMessage()
	: Message(CharDelete)
{
}

CharDeleteMessage::CharDeleteMessage(Position position)
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

Position CharDeleteMessage::getPosition() const
{
	return m_fPos;
}


/*************** CHAR FORMAT MESSAGE ***************/

CharFormatMessage::CharFormatMessage()
	: Message(CharFormat)
{
}

CharFormatMessage::CharFormatMessage(Position position, QTextCharFormat fmt)
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

Position CharFormatMessage::getPosition() const
{
	return m_fPos;
}

QTextCharFormat CharFormatMessage::getCharFormat() const
{
	return m_charFmt;
}


/*************** BULK INSERT MESSAGE ***************/

BulkInsertMessage::BulkInsertMessage()
	: Message(BulkInsert), m_flag(false)
{
}

BulkInsertMessage::BulkInsertMessage(QVector<Symbol> symbols, bool isLast, TextBlockID bId, QTextBlockFormat blkFmt)
	: Message(BulkInsert), m_symbols(symbols), m_blockId(bId), m_blockFmt(blkFmt), m_flag(isLast)
{
}

void BulkInsertMessage::writeTo(QDataStream& stream) const
{
	stream << m_symbols << m_blockId << m_blockFmt << m_flag;
}

void BulkInsertMessage::readFrom(QDataStream& stream)
{
	stream >> m_symbols >> m_blockId >> m_blockFmt >> m_flag;
}

QVector<Symbol> BulkInsertMessage::getSymbols() const
{
	return m_symbols;
}

TextBlockID BulkInsertMessage::getBlockId() const
{
	return m_blockId;
}

QTextBlockFormat BulkInsertMessage::getBlockFormat() const
{
	return m_blockFmt;
}

bool BulkInsertMessage::getIsLast() const
{
	return m_flag;
}


/*************** BULK DELETE MESSAGE ***************/

BulkDeleteMessage::BulkDeleteMessage()
	: Message(BulkDelete)
{
}

BulkDeleteMessage::BulkDeleteMessage(QVector<Position> positions)
	: Message(BulkDelete), m_fPositions(positions)
{
}

void BulkDeleteMessage::writeTo(QDataStream& stream) const
{
	stream << m_fPositions;
}

void BulkDeleteMessage::readFrom(QDataStream& stream)
{
	stream >> m_fPositions;
}

QVector<Position> BulkDeleteMessage::getPositions() const
{
	return m_fPositions;
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

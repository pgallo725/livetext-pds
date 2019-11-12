#include "TextBlock.h"


/*************** TEXTBLOCK CLASS ***************/


TextBlock::TextBlock()
	: _blockId(nullptr), _listId(nullptr)
{
}

TextBlock::TextBlock(qint32 blockNum, qint32 authorId, QTextBlockFormat _fmt)
	: _blockId(TextBlockID(blockNum, authorId)), _listId(nullptr)
{
}

TextBlock::TextBlock(TextBlockID blockId, QTextBlockFormat fmt)
	: _blockId(blockId), _blockFormat(fmt), _listId(nullptr)
{
}


void TextBlock::setFormat(QTextBlockFormat fmt)
{
	_blockFormat = fmt;
}

void TextBlock::setBegin(Position fPosBegin)
{
	_fPosBegin = fPosBegin;
}

void TextBlock::setEnd(Position fPosEnd)
{
	_fPosEnd = fPosEnd;
}

void TextBlock::setList(TextListID id)
{
	_listId = id;
}


TextBlockID TextBlock::getId() const
{
	return _blockId;
}

QTextBlockFormat TextBlock::getFormat() const
{
	return _blockFormat;
}

const Position& TextBlock::begin() const
{
	return _fPosBegin;
}

const Position& TextBlock::end() const
{
	return _fPosEnd;
}

TextListID TextBlock::getListId() const
{
	return _listId;
}

bool TextBlock::isEmpty() const
{
	QVector<qint32> nullPos({ -1, -1 });
	return (_fPosBegin == nullPos && _fPosEnd == nullPos);
}


/*************** SERIALIZATION OPERATORS ***************/


// TextBlock deserialization operator
QDataStream& operator>>(QDataStream& in, TextBlock& blk)
{
	in >> blk._blockId >> blk._blockFormat >> blk._fPosBegin >> blk._fPosEnd >> blk._listId;

	return in;
}

// TextBlock serialization operator
QDataStream& operator<<(QDataStream& out, const TextBlock& blk)
{
	out << blk._blockId << blk._blockFormat << blk._fPosBegin << blk._fPosEnd << blk._listId;

	return out;
}
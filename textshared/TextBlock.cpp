#include "TextBlock.h"

#include <QDataStream>


/*************** TEXTBLOCKID CLASS ***************/


TextBlockID::TextBlockID()
	: _number(-1), _authorId(-1)
{
}

TextBlockID::TextBlockID(qint32 num, qint32 uid)
	: _number(num), _authorId(uid)
{
}

TextBlockID::TextBlockID(const TextBlock& block)
	: TextBlockID(block.getId())
{
}

TextBlockID::TextBlockID(std::nullptr_t)
	: TextBlockID()
{
}


qint32 TextBlockID::getBlockNumber() const
{
	return _number;
}

qint32 TextBlockID::getAuthorId() const
{
	return _authorId;
}


bool TextBlockID::operator<(const TextBlockID& other) const noexcept
{
	return this->_number < other._number ||
		(this->_number == other._number && this->_authorId < other._authorId);
}

bool TextBlockID::operator>(const TextBlockID& other) const noexcept
{
	return this->_number > other._number ||
		(this->_number == other._number && this->_authorId > other._authorId);;
}

bool TextBlockID::operator==(const TextBlockID& other) const noexcept
{
	return (this->_number == other._number && 
		this->_authorId == other._authorId);
}

TextBlockID::operator bool() const noexcept
{
	return !(_number == -1 && _authorId == -1);
}


/*************** TEXTBLOCK CLASS ***************/


TextBlock::TextBlock()
	: _blockId(TextBlockID(nullptr)), _fPosBegin({ -1, -1 }), _fPosEnd({ -1, -1 })
{
}

TextBlock::TextBlock(qint32 blockNum, qint32 authorId, QTextBlockFormat _fmt)
	: _blockId(TextBlockID(blockNum, authorId)), _fPosBegin({ -1, -1 }), _fPosEnd({ -1, -1 })
{
}

TextBlock::TextBlock(TextBlockID blockId, QTextBlockFormat fmt)
	: _blockId(blockId), _blockFormat(fmt), _fPosBegin({ -1, -1 }), _fPosEnd({ -1, -1 })
{
}

void TextBlock::setFormat(QTextBlockFormat fmt)
{
	_blockFormat = fmt;
}

void TextBlock::setBegin(QVector<qint32> fPosBegin)
{
	_fPosBegin = fPosBegin;
}

void TextBlock::setEnd(QVector<qint32> fPosEnd)
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

QVector<qint32> TextBlock::begin() const
{
	return _fPosBegin;
}

QVector<qint32> TextBlock::end() const
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


// TextBlockID deserialization operator
QDataStream& operator>>(QDataStream& in, TextBlockID& bid)
{
	in >> bid._number >> bid._authorId;

	return in;
}

// TextBlockID serialization operator
QDataStream& operator<<(QDataStream& out, const TextBlockID& bid)
{
	out << bid._number << bid._authorId;

	return out;
}

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

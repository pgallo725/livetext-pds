#include "TextBlock.h"

#include <QDataStream>


/*************** TEXTLIST CLASS ***************/


TextList::TextList()
	: _listId(-1), _nBlocks(0)
{
}

TextList::TextList(qint32 listId, QTextListFormat fmt)
	: _listId(listId), _format(fmt), _nBlocks(0)
{
}

void TextList::setFormat(QTextListFormat fmt)
{
	_format = fmt;
}

void TextList::incrementBlocks(int amount)
{
	_nBlocks += amount;
}

void TextList::decrementBlocks(int amount)
{
	_nBlocks -= amount;
}

qint32 TextList::getId() const
{
	return _listId;
}

QTextListFormat TextList::getFormat() const
{
	return _format;
}

bool TextList::isEmpty() const
{
	return _nBlocks == 0;
}


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
	return (_number != -1 || _authorId != -1);
}


/*************** TEXTBLOCK CLASS ***************/


TextBlock::TextBlock()
	: _blockId(TextBlockID(nullptr)), _fPosBegin({ -1, -1 }), _fPosEnd({ -1, -1 }), _listRef(-1)
{
}

TextBlock::TextBlock(qint32 blockNum, qint32 authorId, QTextBlockFormat _fmt, qint32 listRef)
	: _blockId(TextBlockID(blockNum, authorId)), _fPosBegin({ -1, -1 }), _fPosEnd({ -1, -1 }), _listRef(listRef)
{
}

TextBlock::TextBlock(TextBlockID blockId, QTextBlockFormat fmt, qint32 listRef)
	: _blockId(blockId), _format(fmt), _fPosBegin({ -1, -1 }), _fPosEnd({ -1, -1 }), _listRef(listRef)
{
}


void TextBlock::setFormat(QTextBlockFormat fmt)
{
	_format = fmt;
}

void TextBlock::setBegin(QVector<qint32> fPosBegin)
{
	_fPosBegin = fPosBegin;
}

void TextBlock::setEnd(QVector<qint32> fPosEnd)
{
	_fPosEnd = fPosEnd;
}

void TextBlock::setList(qint32 listId)
{
	_listRef = listId;
}


/*void TextBlock::assignToList(TextList& list)
{
	_listRef = list.getId();
	list.incrementBlocks();
}

void TextBlock::removeFromList(TextList& list)
{
	_listRef = -1;
	list.decrementBlocks();
}*/


TextBlockID TextBlock::getId() const
{
	return _blockId;
}

QTextBlockFormat TextBlock::getFormat() const
{
	return _format;
}

QVector<qint32> TextBlock::begin() const
{
	return _fPosBegin;
}

QVector<qint32> TextBlock::end() const
{
	return _fPosEnd;
}

qint32 TextBlock::getListIdentifier() const
{
	return _listRef;
}

bool TextBlock::isEmpty() const
{
	QVector<qint32> nullPos({ -1, -1 });
	return (_fPosBegin == nullPos && _fPosEnd == nullPos);
}


/*************** SERIALIZATION OPERATORS ***************/


// TextList deserialization operator
QDataStream& operator>>(QDataStream& in, TextList& lst)
{
	in >> lst._listId >> lst._format >> lst._nBlocks;

	return in;
}

// TextList serialization operator
QDataStream& operator<<(QDataStream& out, const TextList& lst)
{
	out << lst._listId << lst._format << lst._nBlocks;

	return out;
}

// TextBlockID serialization operator
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
	in >> blk._blockId >> blk._format >> blk._fPosBegin >> blk._fPosEnd >> blk._listRef;

	return in;
}

// TextBlock serialization operator
QDataStream& operator<<(QDataStream& out, const TextBlock& blk)
{
	out << blk._blockId << blk._format << blk._fPosBegin << blk._fPosEnd << blk._listRef;

	return out;
}

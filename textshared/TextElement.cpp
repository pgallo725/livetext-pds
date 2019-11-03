#include "TextElement.h"

#include <QDataStream>


/*************** TEXTELEMENT ID CLASSES ***************/

TextElementID::TextElementID()
	: _number(-1), _authorId(-1)
{
}

TextElementID::TextElementID(qint32 num, qint32 uid)
	: _number(num), _authorId(uid)
{
}


bool TextElementID::operator<(const TextElementID& other) const noexcept
{
	return this->_number < other._number ||
		(this->_number == other._number && this->_authorId < other._authorId);
}

bool TextElementID::operator>(const TextElementID& other) const noexcept
{
	return this->_number > other._number ||
		(this->_number == other._number && this->_authorId > other._authorId);;
}

bool TextElementID::operator==(const TextElementID& other) const noexcept
{
	return (this->_number == other._number &&
		this->_authorId == other._authorId);
}

bool TextElementID::operator!=(const TextElementID& other) const noexcept
{
	return (this->_number != other._number ||
		this->_authorId != other._authorId);
}

TextElementID::operator bool() const noexcept
{
	return !(_number == -1 && _authorId == -1);
}


// TextBlockID

TextBlockID::TextBlockID()
	: TextElementID()
{
}

TextBlockID::TextBlockID(qint32 num, qint32 uid)
	: TextElementID(num, uid)
{
}

TextBlockID::TextBlockID(std::nullptr_t)
	: TextElementID()
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


// TextListID

TextListID::TextListID()
	: TextElementID()
{
}

TextListID::TextListID(qint32 num, qint32 uid)
	: TextElementID(num, uid)
{
}

TextListID::TextListID(std::nullptr_t)
	: TextElementID()
{
}

qint32 TextListID::getListNumber() const
{
	return _number;
}

qint32 TextListID::getAuthorId() const
{
	return _authorId;
}



/*************** TEXTBLOCK CLASS ***************/


TextBlock::TextBlock()
	: _blockId(nullptr), 
	_fPosBegin({ -1, -1 }), _fPosEnd({ -1, -1 }),
	_listId(nullptr)
{
}

TextBlock::TextBlock(qint32 blockNum, qint32 authorId, QTextBlockFormat _fmt)
	: _blockId(TextBlockID(blockNum, authorId)),
	_fPosBegin({ -1, -1 }), _fPosEnd({ -1, -1 }),
	_listId(nullptr)
{
}

TextBlock::TextBlock(TextBlockID blockId, QTextBlockFormat fmt)
	: _blockId(blockId), _blockFormat(fmt),
	_fPosBegin({ -1, -1 }), _fPosEnd({ -1, -1 }),
	_listId(nullptr)
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



/*************** TEXTLIST CLASS ***************/


TextList::TextList()
	: _listId(TextListID(nullptr))
{
}

TextList::TextList(qint32 listNum, qint32 authorId, QTextListFormat fmt)
	: _listId(TextListID(listNum, authorId)), _listFormat(fmt)
{
}

TextList::TextList(TextListID listId, QTextListFormat fmt)
	: _listId(listId), _listFormat(fmt)
{
}


void TextList::addBlock(TextBlockID id)
{
	_blocks.append(id);
}

void TextList::removeBlock(TextBlockID id)
{
	_blocks.removeAll(id);
}

void TextList::setFormat(QTextListFormat fmt)
{
	_listFormat = fmt;
}


TextListID TextList::getId() const
{
	return _listId;
}

QTextListFormat TextList::getFormat() const
{
	return _listFormat;
}

QList<TextBlockID> TextList::getBlocks() const
{
	// List of *unordered* blocks
	return _blocks;
}

bool TextList::isEmpty() const
{
	return _blocks.count() == 0;
}


/*************** SERIALIZATION OPERATORS ***************/


// TextElementID deserialization operator
QDataStream& operator>>(QDataStream& in, TextElementID& bid)
{
	in >> bid._number >> bid._authorId;

	return in;
}

// TextElementID serialization operator
QDataStream& operator<<(QDataStream& out, const TextElementID& bid)
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

// TextList deserialization operator
QDataStream& operator>>(QDataStream& in, TextList& lst)
{
	in >> lst._listId >> lst._listFormat >> lst._blocks;

	return in;
}

// TextList serialization operator
QDataStream& operator<<(QDataStream& out, const TextList& lst)
{
	out << lst._listId << lst._listFormat << lst._blocks;

	return out;
}

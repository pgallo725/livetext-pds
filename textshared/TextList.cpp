#include "TextList.h"

#include <QDataStream>
#include "TextBlock.h"


/*************** TEXTLISTID CLASS ***************/


TextListID::TextListID()
	: _number(-1), _authorId(-1)
{
}

TextListID::TextListID(qint32 num, qint32 uid)
	: _number(num), _authorId(uid)
{
}

TextListID::TextListID(const TextList& list)
	: TextListID(list.getId())
{
}

TextListID::TextListID(std::nullptr_t)
	: TextListID()
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


bool TextListID::operator<(const TextListID& other) const noexcept
{
	return this->_number < other._number ||
		(this->_number == other._number && this->_authorId < other._authorId);
}

bool TextListID::operator>(const TextListID& other) const noexcept
{
	return this->_number > other._number ||
		(this->_number == other._number && this->_authorId > other._authorId);;
}

bool TextListID::operator==(const TextListID& other) const noexcept
{
	return (this->_number == other._number &&
		this->_authorId == other._authorId);
}

TextListID::operator bool() const noexcept
{
	return !(_number == -1 && _authorId == -1);
}


/*************** TEXTLIST CLASS ***************/


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
	return _blocks;
}


/*************** SERIALIZATION OPERATORS ***************/


// TextListID deserialization operator
QDataStream& operator>>(QDataStream& in, TextListID& lid)
{
	in >> lid._number >> lid._authorId;

	return in;
}

// TextListID serialization operator
QDataStream& operator<<(QDataStream& out, const TextListID& lid)
{
	out << lid._number << lid._authorId;

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

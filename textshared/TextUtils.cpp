#include "TextUtils.h"

#include <QDataStream>


/*************** POSITION CLASS ***************/

Position::Position()
	: posIndexes({ -1, -1 })
{
}

Position::Position(QVector<qint32> values)
	: posIndexes(values)
{
}


qint32 Position::size() const
{
	return posIndexes.size();
}

qint32 Position::operator[](int index) const
{
	return posIndexes.at(index);
}

qint32 Position::getAuthorId() const
{
	// The last element in the fractional position vector is the User ID
	return posIndexes.back();
}


bool Position::operator==(const Position& other) const
{
	return posIndexes == other.posIndexes;
}

bool Position::operator<(const Position& other) const
{
	int minlen = this->posIndexes.size() < other.posIndexes.size() ?
		this->posIndexes.size() : other.posIndexes.size();

	for (int i = 0; i < minlen; i++)
	{
		if (this->posIndexes[i] < other.posIndexes[i])
			return true;
		else if (this->posIndexes[i] > other.posIndexes[i])
			return false;
	}

	return this->posIndexes.size() < other.posIndexes.size();
}

bool Position::operator>(const Position& other) const
{
	return !((*this) < other || (*this) == other);
}


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


/*************** SERIALIZATION OPERATORS ***************/


// Position deserialization operator
QDataStream& operator>>(QDataStream& in, Position& pos)
{
	in >> pos.posIndexes;

	return in;
}

// Position serialization operator
QDataStream& operator<<(QDataStream& out, const Position& pos)
{
	out << pos.posIndexes;

	return out;
}

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
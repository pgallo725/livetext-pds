#include "Symbol.h"

#include <algorithm>
#include <QDataStream>


Symbol::Symbol(QChar symbol, qint32 authorId, QVector<qint32> fractionPos)
	: _symbol(symbol), _fPos(fractionPos)
{
	_fPos.push_back(authorId);	// User ID is added as part of the fractional position to ensure uniqueness
}


bool Symbol::operator==(const Symbol& other)
{
	return _fPos == other._fPos;
}

bool Symbol::operator<(const Symbol& other)
{
	int minlen = std::min(this->_fPos.size(), other._fPos.size());
	for (int i = 0; i < minlen; i++)
	{
		if (this->_fPos[i] < other._fPos[i])
			return true;
		else if (this->_fPos[i] > other._fPos[i])
			return false;
	}

	return this->_fPos.size() < other._fPos.size();
}

bool Symbol::operator>(const Symbol& other)
{
	return !((*this) < other || (*this) == other);
}


QChar Symbol::getChar()
{
	return _symbol;
}

qint32 Symbol::getAuthorId()
{
	// The last element in the fractional position vector is the User ID
	return _fPos.back();
}


Symbol::~Symbol()
{
}


// Deserialization operator
QDataStream& operator>>(QDataStream& in, Symbol& sym)
{
	in >> sym._symbol >> sym._fPos;

	return in;
}

// Serialization operator
QDataStream& operator<<(QDataStream& out, const Symbol& sym)
{
	out << sym._symbol << sym._fPos;

	return out;
}

#include "Symbol.h"

#include <QDataStream>
#include <algorithm>


/*************** SYMBOL METHODS ***************/


Symbol::Symbol()
	: _blockRef(TextBlockID(nullptr))
{
}

Symbol::Symbol(QChar sym, QTextCharFormat fmt, qint32 authorId, QVector<qint32> fractionPos, TextBlockID blockRef)
	: _char(sym), _format(fmt), _fPos(fractionPos), _blockRef(blockRef)
{
	_fPos.push_back(authorId);		// User ID is added as part of the fractional position to ensure uniqueness
}


void Symbol::setFormat(QTextCharFormat fmt)
{
	_format = fmt;
}

void Symbol::setBlock(TextBlockID blockId)
{
	_blockRef = blockId;
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
	return _char;
}

QTextCharFormat Symbol::getFormat()
{
	return _format;
}

TextBlockID Symbol::getBlockId()
{
	return _blockRef;
}

qint32 Symbol::getAuthorId()
{
	// The last element in the fractional position vector is the User ID
	return _fPos.back();
}



// Symbol deserialization operator
QDataStream& operator>>(QDataStream& in, Symbol& sym)
{
	in >> sym._char >> sym._format >> sym._fPos;

	return in;
}

// Symbol serialization operator
QDataStream& operator<<(QDataStream& out, const Symbol& sym)
{
	out << sym._char << sym._format << sym._fPos;

	return out;
}

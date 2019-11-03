#include "Symbol.h"

#include <QDataStream>


/*************** SYMBOL METHODS ***************/


Symbol::Symbol()
	: _blockRef(nullptr)
{
}

Symbol::Symbol(QChar sym, QTextCharFormat fmt, qint32 authorId, QVector<qint32> fractionPos)
	: _char(sym), _format(fmt), _fPos(fractionPos), _blockRef(nullptr)
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
	int minlen = this->_fPos.size() < other._fPos.size() ? 
		this->_fPos.size() : other._fPos.size();

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


QChar Symbol::getChar() const
{
	return _char;
}

QTextCharFormat Symbol::getFormat() const
{
	return _format;
}

QVector<qint32> Symbol::getPosition() const
{
	return _fPos;
}

TextBlockID Symbol::getBlockId() const
{
	return _blockRef;
}

qint32 Symbol::getAuthorId() const
{
	// The last element in the fractional position vector is the User ID
	return _fPos.back();
}



// Symbol deserialization operator
QDataStream& operator>>(QDataStream& in, Symbol& sym)
{
	in >> sym._char >> sym._format >> sym._fPos >> sym._blockRef;

	return in;
}

// Symbol serialization operator
QDataStream& operator<<(QDataStream& out, const Symbol& sym)
{
	out << sym._char << sym._format << sym._fPos << sym._blockRef;

	return out;
}

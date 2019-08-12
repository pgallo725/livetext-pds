#include "Symbol.h"

#include <algorithm>
#include <QDataStream>



Symbol::Symbol(SymbolType type, qint32 authorId, QVector<qint32> fractionPos)
	: _type(type), _fPos(fractionPos)
{
	_fPos.push_back(authorId);		// User ID is added as part of the fractional position to ensure uniqueness
}

Symbol::Symbol(SymbolType type, QChar symbol, QTextCharFormat fmt, qint32 authorId, QVector<qint32> fractionPos)
	: Symbol(type, authorId, fractionPos)
{
	_item = QVariant(symbol);		// Fill the unions with the QChar and its QTextCharFormat attribute
	_format = QVariant(fmt);
}

Symbol::Symbol(SymbolType type, QTextBlockFormat fmt, qint32 authorId, QVector<qint32> fractionPos)
	: Symbol(type, authorId, fractionPos)
{
	_item = QVariant();				// Fill the unions with the QTextBlockFormat attributes
	_format = QVariant(fmt);
}

Symbol::Symbol(SymbolType type, QTextListFormat fmt, qint32 authorId, QVector<qint32> fractionPos)
	: Symbol(type, authorId, fractionPos)
{
	_item = QVariant();				// Fills the unions with the QTextListFormat attribute
	_format = QVariant(fmt);
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

SymbolType Symbol::getType()
{
	return _type;
}

bool Symbol::isChar()
{
	return _type == SymbolType::Char;
}

bool Symbol::isBlockDelimiter()
{
	return _type == SymbolType::BlockBegin || _type == SymbolType::BlockEnd;
}

bool Symbol::isListDelimiter()
{
	return _type == SymbolType::ListBegin || _type == SymbolType::ListEnd;
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
	in >> (qint32&)sym._type >> sym._item >> sym._format >> sym._fPos;

	return in;
}

// Serialization operator
QDataStream& operator<<(QDataStream& out, const Symbol& sym)
{
	out << sym._type << sym._item << sym._format << sym._fPos;

	return out;
}



/********************************************************
*		Type-specific accessor methods (getters)		*
********************************************************/


QChar CharSymbol::getChar()
{
	return _item.value<QChar>();
}

QTextCharFormat CharSymbol::getCharFormat()
{
	return _format.value<QTextCharFormat>();
}


QTextBlockFormat BlockDelimiterSymbol::getBlockFormat()
{
	return _format.value<QTextBlockFormat>();
}


QTextListFormat ListDelimiterSymbol::getListFormat()
{
	return _format.value<QTextListFormat>();
}

#include "Symbol.h"

#include <algorithm>
#include <QDataStream>



Symbol::Symbol(Type type, qint32 authorId, QVector<qint32> fractionPos)
	: _type(type), _fPos(fractionPos)
{
	_fPos.push_back(authorId);		// User ID is added as part of the fractional position to ensure uniqueness
}

Symbol::Symbol(Type type, QChar symbol, QTextCharFormat fmt, qint32 authorId, QVector<qint32> fractionPos)
	: Symbol(type, authorId, fractionPos)
{
	_item = QVariant(symbol);		// Fill the unions with the QChar and its QTextCharFormat attribute
	_format = QVariant(fmt);
}

Symbol::Symbol(Type type, DelimiterType delimiter, QTextBlockFormat fmt, qint32 authorId, QVector<qint32> fractionPos)
	: Symbol(type, authorId, fractionPos)
{
	_item = QVariant(delimiter);	// Fill the unions with the TextBlock delimiter type and the QTextBlockFormat attributes
	_format = QVariant(fmt);
}

Symbol::Symbol(Type type, Symbol::DelimiterType delimiter, QTextListFormat fmt, qint32 authorId, QVector<qint32> fractionPos)
	: Symbol(type, authorId, fractionPos)
{
	_item = QVariant(delimiter);	// Fills the unions with the TextList delimiter type and the QTextListFormat attribute
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


Symbol::Type Symbol::getType()
{
	return _type;
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


QChar Char::getChar()
{
	return _item.value<QChar>();
}

QTextCharFormat Char::getCharFormat()
{
	return _format.value<QTextCharFormat>();
}


Symbol::DelimiterType BlockDelimiter::getDelimiterType()
{
	return _item.value<Symbol::DelimiterType>();
}

QTextBlockFormat BlockDelimiter::getBlockFormat()
{
	return _format.value<QTextBlockFormat>();
}


Symbol::DelimiterType ListDelimiter::getDelimiterType()
{
	return _item.value<Symbol::DelimiterType>();
}

QTextListFormat ListDelimiter::getListFormat()
{
	return _format.value<QTextListFormat>();
}

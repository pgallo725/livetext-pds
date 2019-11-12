#include "Symbol.h"

#include <QDataStream>


/*************** SYMBOL METHODS ***************/

Symbol::Symbol()
	: _blockRef(nullptr)
{
}

Symbol::Symbol(QChar sym, QTextCharFormat fmt, Position fractionPos)
	: _char(sym), _format(fmt), _fPos(fractionPos), _blockRef(nullptr)
{
}


void Symbol::setFormat(QTextCharFormat fmt)
{
	_format = fmt;
}

void Symbol::setBlock(TextBlockID blockId)
{
	_blockRef = blockId;
}


QChar Symbol::getChar() const
{
	return _char;
}

QTextCharFormat Symbol::getFormat() const
{
	return _format;
}

const Position& Symbol::getPosition() const
{
	return _fPos;
}

TextBlockID Symbol::getBlockId() const
{
	return _blockRef;
}

qint32 Symbol::getAuthorId() const
{
	return _fPos.getAuthorId();
}


/*************** SERIALIZATION OPERATORS ***************/


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

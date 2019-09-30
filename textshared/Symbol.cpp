#include "Symbol.h"

#include <algorithm>
#include <QDataStream>



/*************** TEXTLIST METHODS ***************/


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


/*************** TEXTBLOCK METHODS ***************/


TextBlock::TextBlock()
	: _blockId(qMakePair(-1, -1)), _nChars(0), _listRef(-1)
{
}

TextBlock::TextBlock(qint32 blockNum, qint32 authorId, QTextBlockFormat _fmt, qint32 listRef)
	: _blockId(qMakePair(blockNum, authorId)), _nChars(0), _listRef(listRef)
{
}

TextBlock::TextBlock(QPair<qint32, qint32> blockIdPair, QTextBlockFormat fmt, qint32 listRef)
	: _blockId(blockIdPair), _format(fmt), _listRef(listRef)
{
}

void TextBlock::setFormat(QTextBlockFormat fmt)
{
	_format = fmt;
}

void TextBlock::incrementSymbols(int amount)
{
	_nChars += amount;
}

void TextBlock::decrementSymbols(int amount)
{
	_nChars -= amount;
}

void TextBlock::assignToList(TextList& list)
{
	_listRef = list.getId();
	list.incrementBlocks();
}

void TextBlock::removeFromList(TextList& list)
{
	_listRef = -1;
	list.decrementBlocks();
}

qint32 TextBlock::getBlockNumber() const
{
	return _blockId.first;
}

qint32 TextBlock::getAuthorId() const
{
	return _blockId.second;
}

QPair<qint32, qint32> TextBlock::getIdPair() const
{
	return _blockId;
}

QTextBlockFormat TextBlock::getFormat() const
{
	return _format;
}

qint32 TextBlock::getListIdentifier() const
{
	return _listRef;
}

int TextBlock::size() const
{
	return _nChars;
}

bool TextBlock::isEmpty() const
{
	return _nChars == 0;
}


/*************** SYMBOL METHODS ***************/


Symbol::Symbol()
	: _blockRef(qMakePair(-1, -1))
{
}

Symbol::Symbol(QChar sym, QTextCharFormat fmt, qint32 authorId, QVector<qint32> fractionPos)
	: _char(sym), _format(fmt), _fPos(fractionPos), _blockRef(qMakePair(-1, -1))
{
	_fPos.push_back(authorId);		// User ID is added as part of the fractional position to ensure uniqueness
}


void Symbol::setFormat(QTextCharFormat fmt)
{
	_format = fmt;
}

void Symbol::assignToBlock(TextBlock& block)
{
	_blockRef = block.getIdPair();
	block.incrementSymbols();
}

void Symbol::removeFromBlock(TextBlock& block)
{
	_blockRef = qMakePair(-1, -1);
	block.decrementSymbols();
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

QPair<qint32, qint32> Symbol::getBlockIdentifier()
{
	return _blockRef;
}

qint32 Symbol::getAuthorId()
{
	// The last element in the fractional position vector is the User ID
	return _fPos.back();
}


// TextList deserialization operator
QDataStream& operator>>(QDataStream& in, TextList& sym)
{
	in >> sym._listId >> sym._format >> sym._nBlocks;

	return in;
}

// TextList serialization operator
QDataStream& operator<<(QDataStream& out, const TextList& sym)
{
	out << sym._listId << sym._format << sym._nBlocks;

	return out;
}

// TextBlock deserialization operator
QDataStream& operator>>(QDataStream& in, TextBlock& sym)
{
	in >> sym._blockId >> sym._format >> sym._nChars >> sym._listRef;

	return in;
}

// TextBlock serialization operator
QDataStream& operator<<(QDataStream& out, const TextBlock& sym)
{
	out << sym._blockId << sym._format << sym._nChars << sym._listRef;

	return out;
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

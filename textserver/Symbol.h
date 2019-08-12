#pragma once

#include <QVector>
#include <QVariant>
#include <QChar>
#include <QTextCharFormat>
#include <QTextBlockFormat>
#include <QTextListFormat>


enum SymbolType
{
	Char,
	BlockBegin,
	BlockEnd,
	ListBegin,
	ListEnd,
	None
};


class Symbol
{

	/* Operators for QDataStream serialization and deserialization */
	friend QDataStream& operator>>(QDataStream& in, Symbol& sym);				// Input
	friend QDataStream& operator<<(QDataStream& out, const Symbol& sym);		// Output

protected:

	SymbolType _type;
	QVariant _item;		// { QChar, DelimiterType }
	QVariant _format;	// { QTextCharFormat, QTextBlockFormat, QTextListFormat }

public:

	QVector<qint32> _fPos;

protected:

	// Constructor which handles the fields that are in common to all symbol types
	Symbol(SymbolType type, qint32 authorId, QVector<qint32> fractionPos);

public:

	/* Constructors */

	Symbol() : _type(None) { };		// Use this to construct an empty symbol and populate the fields later

	Symbol(SymbolType type, QChar sym, QTextCharFormat fmt, qint32 authorId, QVector<qint32> fractionPos);
	Symbol(SymbolType type, QTextBlockFormat fmt, qint32 authorId, QVector<qint32> fractionPos);
	Symbol(SymbolType type, QTextListFormat fmt, qint32 authorId, QVector<qint32> fractionPos);

	/* Comparison operators */
	bool operator==(const Symbol& other);
	bool operator<(const Symbol& other);
	bool operator>(const Symbol& other);

	/* Getters */
	SymbolType getType();
	bool isChar();
	bool isBlockDelimiter();
	bool isListDelimiter();
	qint32 getAuthorId();
	

	~Symbol();

};


// Needed to make these types available to templates objects such as QVariant

Q_DECLARE_METATYPE(QTextCharFormat);
Q_DECLARE_METATYPE(QTextBlockFormat);
Q_DECLARE_METATYPE(QTextListFormat);



/************************************************************
*		Interfaces for handling specific Symbol types		*
************************************************************/

class CharSymbol : public Symbol
{

public:

	QChar getChar();
	QTextCharFormat getCharFormat();

};


class BlockDelimiterSymbol : public Symbol
{

public:

	QTextBlockFormat getBlockFormat();

};


class ListDelimiterSymbol : public Symbol
{

public:

	QTextListFormat getListFormat();

};
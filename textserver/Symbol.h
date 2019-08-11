#pragma once

#include <QVector>
#include <QVariant>
#include <QChar>
#include <QTextCharFormat>
#include <QTextBlockFormat>
#include <QTextListFormat>


class Symbol
{

	/* Operators for QDataStream serialization and deserialization */
	friend QDataStream& operator>>(QDataStream& in, Symbol& sym);				// Input
	friend QDataStream& operator<<(QDataStream& out, const Symbol& sym);		// Output

public:

	enum Type 
	{
		Char,
		ListDelimiter,
		BlockDelimiter,
		None
	};

	enum DelimiterType
	{
		Begin,
		End
	};

protected:

	Type _type;
	QVariant _item;		// { QChar, DelimiterType }
	QVariant _format;	// { QTextCharFormat, QTextBlockFormat, QTextListFormat }

public:

	QVector<qint32> _fPos;

protected:

	// Constructor which handles the fields that are in common to all symbol types
	Symbol(Type type, qint32 authorId, QVector<qint32> fractionPos);

public:

	/* Constructors */

	Symbol() : _type(None) { };		// Use this to construct an empty symbol and populate the fields later

	Symbol(Symbol::Type type, QChar sym, QTextCharFormat fmt, qint32 authorId, QVector<qint32> fractionPos);
	Symbol(Symbol::Type type, DelimiterType delimiter, QTextBlockFormat fmt, qint32 authorId, QVector<qint32> fractionPos);
	Symbol(Symbol::Type type, DelimiterType delimiter, QTextListFormat fmt, qint32 authorId, QVector<qint32> fractionPos);

	/* Comparison operators */
	bool operator==(const Symbol& other);
	bool operator<(const Symbol& other);
	bool operator>(const Symbol& other);

	/* Getters */
	Symbol::Type getType();
	qint32 getAuthorId();
	

	~Symbol();

};


/************************************************************
*		Interfaces for handling specific Symbol types		*
************************************************************/

class Char : public Symbol
{

public:

	QChar getChar();
	QTextCharFormat getCharFormat();

};


class BlockDelimiter : public Symbol
{

public:

	Symbol::DelimiterType getDelimiterType();
	QTextBlockFormat getBlockFormat();

};


class ListDelimiter : public Symbol
{

public:

	Symbol::DelimiterType getDelimiterType();
	QTextListFormat getListFormat();

};
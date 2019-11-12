#pragma once

#include <QChar>
#include <QTextCharFormat>
#include "TextUtils.h"


class Symbol
{
	/* Operators for QDataStream serialization and deserialization */
	friend QDataStream& operator>>(QDataStream& in, Symbol& sym);				// Input
	friend QDataStream& operator<<(QDataStream& out, const Symbol& sym);		// Output

private:

	QChar _char;
	QTextCharFormat _format;
	Position _fPos;
	TextBlockID _blockRef;

public:

	Symbol();		// Constructs an empty Symbol (for deserialization purposes only)

	Symbol(QChar sym, QTextCharFormat fmt, Position fractionPos);


	/* setters */
	void setFormat(QTextCharFormat fmt);
	void setBlock(TextBlockID blockId);

	/* getters */
	QChar getChar() const;
	QTextCharFormat getFormat() const;
	const Position& getPosition() const;
	TextBlockID getBlockId() const;
	qint32 getAuthorId() const;
};

Q_DECLARE_METATYPE(Symbol);

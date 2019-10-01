#pragma once

#include <QVector>
#include <QChar>
#include <QTextCharFormat>
#include "TextBlock.h"


class Symbol
{
	/* Operators for QDataStream serialization and deserialization */
	friend QDataStream& operator>>(QDataStream& in, Symbol& sym);				// Input
	friend QDataStream& operator<<(QDataStream& out, const Symbol& sym);		// Output

private:

	QChar _char;
	QTextCharFormat _format;
	TextBlockID _blockRef;

public:

	QVector<qint32> _fPos;

public:

	Symbol();		// Use this to construct an empty Symbol and populate the fields later

	Symbol(QChar sym, QTextCharFormat fmt, qint32 authorId, QVector<qint32> fractionPos, TextBlockID blockRef = nullptr);


	/* setters */
	void setFormat(QTextCharFormat fmt);
	void setBlock(TextBlockID blockId);

	/* comparison operators */
	bool operator==(const Symbol& other);
	bool operator<(const Symbol& other);
	bool operator>(const Symbol& other);

	/* getters */
	QChar getChar();
	QTextCharFormat getFormat();
	TextBlockID getBlockId();
	qint32 getAuthorId();
};

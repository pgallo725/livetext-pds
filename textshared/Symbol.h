#pragma once

#include <QChar>
#include <QTextCharFormat>
#include "TextElement.h"


class Symbol
{
	friend class Document;

	/* Operators for QDataStream serialization and deserialization */
	friend QDataStream& operator>>(QDataStream& in, Symbol& sym);				// Input
	friend QDataStream& operator<<(QDataStream& out, const Symbol& sym);		// Output

private:

	QChar _char;
	QTextCharFormat _format;
	QVector<qint32> _fPos;
	TextBlockID _blockRef;

public:

	Symbol();		// Constructs an empty Symbol (for deserialization purposes only)

	Symbol(QChar sym, QTextCharFormat fmt, qint32 authorId, QVector<qint32> fractionPos);


	/* setters */
	void setFormat(QTextCharFormat fmt);
	void setBlock(TextBlockID blockId);

	/* comparison operators */
	bool operator==(const Symbol& other);
	bool operator<(const Symbol& other);
	bool operator>(const Symbol& other);

	/* getters */
	QChar getChar() const;
	QTextCharFormat getFormat() const;
	QVector<qint32> getPosition() const;
	TextBlockID getBlockId() const;
	qint32 getAuthorId() const;
};

Q_DECLARE_METATYPE(Symbol);

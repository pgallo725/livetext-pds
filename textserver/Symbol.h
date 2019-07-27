#pragma once

#include <QChar>
#include <QVector>

class Symbol
{
	/* Operators for QDataStream serialization and deserialization */
	friend QDataStream& operator>>(QDataStream& in, Symbol& sym);				// Input
	friend QDataStream& operator<<(QDataStream& out, const Symbol& sym);		// Output

private:

	QChar _symbol;

public:

	QVector<qint32> _fPos;

public:

	Symbol() { };	// Use this to construct an empty symbol and populate the fields later

	Symbol(QChar symbol, qint32 authorId, QVector<qint32> fractionPos);

	bool operator==(const Symbol& other);
	bool operator<(const Symbol& other);
	bool operator>(const Symbol& other);

	QChar getChar();
	qint32 getAuthorId();

	~Symbol();

};
#pragma once

#include <QString>

#include "Symbol.h"


class Document
{
	friend class DocumentReadyMessage;

	/* Operators for QDataStream serialization and deserialization */
	friend QDataStream& operator>>(QDataStream& in, Document& user);			// Input
	friend QDataStream& operator<<(QDataStream& out, const Document& user);		// Output

private:

	QString URI;
	QStringList editors;
	QVector<Symbol> _text;	// Actual document contents

	static const int fPosGapSize = 4;

protected:

	Document();		// Only use this to construct an empty Document object for deserialization purposes

public:

	Document(QString uri);
	~Document();

	void load();
	void save();

	void insert(Symbol s);
	void remove(const Symbol& s);
	void removeAt(QVector<qint32> fPos);

	QVector<qint32> fractionalPosBegin();
	QVector<qint32> fractionalPosEnd();
	QVector<qint32> fractionalPosBetween(int prev_i, int next_i);

	/* getters */
	QString getName();
	QString getURI();
	QString getAuthor();

	QString toString();		// returns a printable representation of the document's contents

	void insertNewEditor(QString edit);

private:

	int binarySearch(QVector<qint32> position);
};


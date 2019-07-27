#pragma once

#include <QString>
#include "Symbol.h"

class Document
{
private:

	QString docName;
	QString URI;
	QString creatorName;

	// Document contents
	QVector<Symbol> _text;

	static const int fPosGapSize = 4;

public:

	Document(QString name, QString uri, QString userName);
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
	QString getURI();

private:

	int binarySearch(QVector<qint32> position);
};


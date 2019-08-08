#pragma once

//#include <QObject>
#include <QString>
#include <QStringList>
#include <QVector>
#include "Symbol.h"

class Document //: public QObject
{
//	Q_OBJECT

private:

	QString URI;
	QStringList editors;
	// Document actual content
	QVector<Symbol> _text;

	static const int fPosGapSize = 4;

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

	void insertNewEditor(QString edit);

private:

	int binarySearch(QVector<qint32> position);
};


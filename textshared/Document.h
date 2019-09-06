#pragma once

#include <QString>
#include "Symbol.h"

class URI
{
	/* Operators for QDataStream serialization and deserialization */
	friend QDataStream& operator>>(QDataStream& in, URI& uri);				// Input
	friend QDataStream& operator<<(QDataStream& out, const URI& uri);		// Output

private:
	
	QString str;

public:

	URI();
	URI(QString docURI);

	QString toString();
	std::string toStdString();

	QString getAuthorName();		// extract the document's author name from the URI
	QString getDocumentName();		// get the document name from the URI


	bool operator<(const URI& other) const noexcept;		// Comparators so that the URI type can be used as a key in QMap collections
	bool operator>(const URI& other) const noexcept;
	bool operator==(const URI& other) const noexcept;		// or be stored in a QList

};



class Document
{
	friend class DocumentReadyMessage;

	/* Operators for QDataStream serialization and deserialization */
	friend QDataStream& operator>>(QDataStream& in, Document& user);			// Input
	friend QDataStream& operator<<(QDataStream& out, const Document& user);		// Output

private:

	URI uri;
	QStringList editors;
	QVector<Symbol> _text;	// Actual document contents

	static const int fPosGapSize = 4;

protected:

	Document();		// Only use this to construct an empty Document object for deserialization purposes

public:

	Document(URI uri);
	~Document();

	void load();
	void unload();
	void save();

	void insert(Symbol s);
	void remove(const Symbol& s);
	QVector<qint32> removeAt(QVector<qint32> fPos);
	QVector<qint32> removeAtIndex(int index);

	QVector<qint32> fractionalPosBegin();
	QVector<qint32> fractionalPosEnd();
	QVector<qint32> fractionalPosBetween(int prev_i, int next_i);
	QVector<qint32> fractionalPosAtIndex(int index);

	/* getters */
	URI getURI();
	QString getName();
	QString getAuthor();

	QVector<Symbol> getContent();

	QString toString();		// returns a printable representation of the document's contents

	void insertNewEditor(QString edit);

private:

	int binarySearch(QVector<qint32> position);
};


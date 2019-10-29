#pragma once

#include <QString>

#include "Symbol.h"
#include "TextBlock.h"
#include "TextList.h"


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

	QString getAuthorName() const;		// extract the document's author name from the URI
	QString getDocumentName() const;	// get the document name from the URI


	bool operator<(const URI& other) const noexcept;		// Comparators so that the URI type can be used as a key in QMap collections
	bool operator>(const URI& other) const noexcept;
	bool operator==(const URI& other) const noexcept;		// or be stored in a QList

};

Q_DECLARE_METATYPE(URI);


class Document
{
	friend class DocumentEditor;
	friend class DocumentReadyMessage;

	/* Operators for QDataStream serialization and deserialization */
	friend QDataStream& operator>>(QDataStream& in, Document& user);			// Input
	friend QDataStream& operator<<(QDataStream& out, const Document& user);		// Output

private:

	URI uri;

	QVector<Symbol> _text;		// Actual document contents

	qint32 _blockCounter;
	QMap<TextBlockID, TextBlock> _blocks;

	qint32 _listCounter;
	QMap<TextListID, TextList> _lists;

	static const int fPosGapSize = 4;

protected:

	Document();		// Only use this to construct an empty Document object for deserialization purposes

public:

	Document(URI uri, qint32 authorId = -1);
	~Document();

	void load();
	void unload();
	void save();
	void erase();

	Symbol& operator[](QVector<qint32> fPos);
	Symbol& operator[](int pos);

	int insert(Symbol& s);
	int remove(QVector<qint32> fPos);
	QVector<qint32> removeAtIndex(int index);
	int editBlockList(TextBlockID bId, TextListID lId, QTextListFormat fmt);	// adds or removes the block from the list (creating it if new)
	int formatSymbol(QVector<qint32> fPos, QTextCharFormat fmt);
	int formatBlock(TextBlockID id, QTextBlockFormat fmt);
	int formatList(TextListID id, QTextListFormat fmt);

	QVector<qint32> fractionalPosBegin();
	QVector<qint32> fractionalPosEnd();
	QVector<qint32> fractionalPosBetween(int prev_i, int next_i);
	QVector<qint32> fractionalPosAtIndex(int index);

	/* getters */
	URI getURI() const;
	QString getName() const;
	QString getAuthor() const;

	int length();
	QVector<Symbol> getContent();
	QString toString();				// returns a printable representation of the document's contents

	TextBlock& getBlock(TextBlockID id);
	int getBlockPosition(TextBlockID blockId);
	TextBlockID getBlockAt(int index);
	QList<TextBlockID> getBlocksBetween(int start, int end);

	TextList& getList(TextListID id);
	int getListPosition(TextListID listId);
	TextListID getListAt(int index);
	QList<TextBlockID> getListBlocksInOrder(TextListID listId);

private:

	int binarySearch(QVector<qint32> position);

	void addCharToBlock(Symbol& s, TextBlock& b);
	void removeCharFromBlock(Symbol& s, TextBlock& b);

	void addBlockToList(TextBlock& b, TextList& l);
	void removeBlockFromList(TextBlock& b, TextList& l);
};


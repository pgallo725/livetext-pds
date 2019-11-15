#pragma once

#include <QString>

#include "Symbol.h"
#include "TextBlock.h"
#include "TextList.h"

/* Symbol reserved to concatenate (and then split) different information in the URI
   NOTE: used in regular expressions, prepend a \ if an escape is needed */
#define URI_FIELD_SEPARATOR	'_'		
#define MAX_DOCNAME_LENGTH 100		// characters

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

	/* Operators for QDataStream serialization and deserialization */
	friend QDataStream& operator>>(QDataStream& in, Document& user);			// Input
	friend QDataStream& operator<<(QDataStream& out, const Document& user);		// Output

private:

	URI uri;

	QVector<Symbol> _text;

	qint32 _blockCounter;
	QMap<TextBlockID, TextBlock> _blocks;

	qint32 _listCounter;
	QMap<TextListID, TextList> _lists;

public:

	Document();		// Only use to construct an empty Document object for deserialization purposes

	Document(URI uri, qint32 authorId = -1);
	~Document();

	/* File methods */
	void load();
	void unload();
	void save();
	void erase();

	/* Editing methods */
	int insert(Symbol& s);
	int remove(const Position& fPos);
	Position removeAtIndex(int index);

	int editBlockList(TextBlockID bId, TextListID lId, QTextListFormat fmt);
	int formatSymbol(const Position& fPos, QTextCharFormat fmt);
	int formatBlock(TextBlockID id, QTextBlockFormat fmt);
	int formatList(TextListID id, QTextListFormat fmt);
	

	/* Getters */
	URI getURI() const;
	QString getName() const;
	QString getAuthor() const;

	int length() const;
	QVector<Symbol> getContent() const;
	QString toString() const;				// returns a printable representation of the document's contents

	// The [] array operator works with both indexes and fractional positions, and it
	// returns the corresponding symbols in the document
	Symbol& operator[](const Position& fPos);
	Symbol& operator[](int pos);

	/* Block accessor methods */
	TextBlock& getBlock(TextBlockID id);
	int getBlockPosition(TextBlockID blockId);
	TextBlockID getBlockAt(int index);
	QList<TextBlockID> getBlocksBetween(int start, int end);

	/* List accessor methods */
	TextList& getList(TextListID id);
	int getListPosition(TextListID listId);
	TextListID getListAt(int index);
	QList<TextBlockID> getOrderedListBlocks(TextListID listId);


private:

	/* Binary search methods to translate from a fractional position to an integer index */
	int positionIndex(const Position& pos);
	int insertionIndex(const Position& pos);

	/* Fractional position algorithm */
	Position newFractionalPos(int index, qint32 _userId);

	// Internal handling of chars and blocks relationships
	void addCharToBlock(Symbol& s, TextBlock& b);
	void removeCharFromBlock(Symbol& s, TextBlock& b);

	// Internal handling of blocks and lists relationships
	void addBlockToList(TextBlock& b, TextList& l);
	void removeBlockFromList(TextBlock& b, TextList& l);
};

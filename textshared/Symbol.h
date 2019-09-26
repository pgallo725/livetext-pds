#pragma once

#include <QVector>
#include <QChar>
#include <QTextCharFormat>
#include <QTextBlockFormat>
#include <QTextListFormat>

// Needed to make these types available to templates objects such as QVariant

Q_DECLARE_METATYPE(QTextCharFormat);
Q_DECLARE_METATYPE(QTextBlockFormat);
Q_DECLARE_METATYPE(QTextListFormat);



class TextList
{
	/* Operators for QDataStream serialization and deserialization */
	friend QDataStream& operator>>(QDataStream& in, TextList& sym);				// Input
	friend QDataStream& operator<<(QDataStream& out, const TextList& sym);		// Output

private:

	qint32 _listId;
	QTextListFormat _format;
	qint32 _nBlocks;

public:

	TextList();		// Empty constructor, to use before populating fields with deserialization

	TextList(qint32 listId, QTextListFormat fmt);

	/* setters */
	void setFormat(QTextListFormat fmt);
	void incrementBlocks(int amount = 1);
	void decrementBlocks(int amount = 1);

	/* getters */
	qint32 getId() const;
	QTextListFormat getFormat() const;
	bool isEmpty() const;

};



class TextBlock
{
	/* Operators for QDataStream serialization and deserialization */
	friend QDataStream& operator>>(QDataStream& in, TextBlock& sym);			// Input
	friend QDataStream& operator<<(QDataStream& out, const TextBlock& sym);		// Output

private:

	qint32 _blockId;
	qint32 _authorId;
	QTextBlockFormat _format;
	qint32 _nChars;
	qint32 _listRef;

public:

	TextBlock();		// Empty constructor, to use before populating fields with deserialization

	TextBlock(qint32 blockId, qint32 authorId, QTextBlockFormat _fmt, qint32 listRef = -1);

	/* setters */
	void setFormat(QTextBlockFormat fmt);
	void incrementSymbols(int amount = 1);
	void decrementSymbols(int amount = 1);
	void assignToList(TextList& list);
	void removeFromList(TextList& list);

	/* getters */
	qint32 getId() const;
	qint32 getAuthorId() const;
	QPair<qint32, qint32> getIdPair() const;
	QTextBlockFormat getFormat() const;
	qint32 getListIdentifier() const;
	int size() const;
	bool isEmpty() const;

};



class Symbol
{

	/* Operators for QDataStream serialization and deserialization */
	friend QDataStream& operator>>(QDataStream& in, Symbol& sym);				// Input
	friend QDataStream& operator<<(QDataStream& out, const Symbol& sym);		// Output

private:

	QChar _char;
	QTextCharFormat _format;
	QPair<qint32, qint32> _blockRef;

public:

	QVector<qint32> _fPos;

public:

	Symbol();		// Use this to construct an empty Symbol and populate the fields later

	Symbol(QChar sym, QTextCharFormat fmt, qint32 authorId, QVector<qint32> fractionPos);


	/* setters */
	void setFormat(QTextCharFormat fmt);
	void assignToBlock(TextBlock& block);
	void removeFromBlock(TextBlock& block);

	/* comparison operators */
	bool operator==(const Symbol& other);
	bool operator<(const Symbol& other);
	bool operator>(const Symbol& other);

	/* getters */
	QChar getChar();
	QTextCharFormat getFormat();
	QPair<qint32, qint32> getBlockIdentifier();
	qint32 getAuthorId();

};

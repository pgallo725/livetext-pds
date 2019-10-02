#pragma once

#include <QTextBlockFormat>
#include <QTextListFormat>

class TextBlockID;
class TextBlock;


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


class TextBlockID
{
	/* Operators for QDataStream serialization and deserialization */
	friend QDataStream& operator>>(QDataStream& in, TextBlockID& bid);			// Input
	friend QDataStream& operator<<(QDataStream& out, const TextBlockID& bid);	// Output

private:

	qint32 _number;
	qint32 _authorId;

public:

	TextBlockID();				// Empty constructor for deserialization purposes

	TextBlockID(qint32 num, qint32 uid);		// Construct a TextBlockID with the specified pair of values
	TextBlockID(const TextBlock& block);		// Copy the TextBlockID of the reference block
	TextBlockID(std::nullptr_t);				// Construct a null TextBlockID

	qint32 getBlockNumber() const;
	qint32 getAuthorId() const;

	// Comparators to use this type as a key in QMap collections, or store in a QList
	bool operator<(const TextBlockID& other) const noexcept;		
	bool operator>(const TextBlockID& other) const noexcept;
	bool operator==(const TextBlockID& other) const noexcept;
	operator bool() const noexcept;
};

Q_DECLARE_METATYPE(TextBlockID);


class TextBlock
{
	/* Operators for QDataStream serialization and deserialization */
	friend QDataStream& operator>>(QDataStream& in, TextBlock& sym);			// Input
	friend QDataStream& operator<<(QDataStream& out, const TextBlock& sym);		// Output

private:

	TextBlockID _blockId;
	QTextBlockFormat _format;
	QVector<qint32> _fPosBegin;
	QVector<qint32> _fPosEnd;

	qint32 _listRef;

public:

	TextBlock();		// Empty constructor, to use before populating fields with deserialization

	TextBlock(qint32 blockNum, qint32 authorId, QTextBlockFormat fmt, qint32 listRef = -1);
	TextBlock(TextBlockID blockId, QTextBlockFormat fmt, qint32 listRef = -1);

	/* setters */
	void setFormat(QTextBlockFormat fmt);
	void setBegin(QVector<qint32> fPosBegin);
	void setEnd(QVector<qint32> fPosEnd);
	void setList(qint32 listId);

	/*void assignToList(TextList& list);
	void removeFromList(TextList& list);*/

	/* getters */
	TextBlockID getId() const;
	QTextBlockFormat getFormat() const;
	QVector<qint32> begin() const;
	QVector<qint32> end() const;
	qint32 getListIdentifier() const;
	bool isEmpty() const;

};

Q_DECLARE_METATYPE(TextBlock);

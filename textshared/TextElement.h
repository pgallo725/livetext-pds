#pragma once

#include <QTextBlockFormat>
#include <QTextListFormat>


class TextElementID
{
protected:

	qint32 _number;
	qint32 _authorId;

	/* Operators for QDataStream serialization and deserialization */
	friend QDataStream& operator>>(QDataStream& in, TextElementID& bid);			// Input
	friend QDataStream& operator<<(QDataStream& out, const TextElementID& bid);		// Output

	TextElementID();		// Empty constructor for deserialization purposes

public:

	// Construct a TextElementID with the specified pair of values
	TextElementID(qint32 num, qint32 uid);		

	virtual qint32 getAuthorId() const = 0;		// To make the TextElemendID class abstract

	// Comparators to use this type as a key in QMap collections, or store in a QList
	bool operator<(const TextElementID& other) const noexcept;
	bool operator>(const TextElementID& other) const noexcept;
	bool operator==(const TextElementID& other) const noexcept;
	bool operator!=(const TextElementID& other) const noexcept;
	operator bool() const noexcept;
};


class TextBlockID : public TextElementID
{
public:

	TextBlockID();		// Empty constructor for deserialization purposes

	TextBlockID(qint32 num, qint32 uid);		// Construct a TextBlockID with the specified pair of values
	TextBlockID(std::nullptr_t);				// Construct a null TextBlockID

	qint32 getBlockNumber() const;
	qint32 getAuthorId() const override;
};

Q_DECLARE_METATYPE(TextBlockID);


class TextListID : public TextElementID
{
public:

	TextListID();		// Empty constructor for deserialization purposes

	TextListID(qint32 num, qint32 uid);		// Construct a TextListID with the specified pair of values
	TextListID(std::nullptr_t);				// Construct a null TextListID

	qint32 getListNumber() const;
	qint32 getAuthorId() const override;
};

Q_DECLARE_METATYPE(TextListID);



/************* TEXTBLOCK CLASS *************/

class TextBlock
{
	/* Operators for QDataStream serialization and deserialization */
	friend QDataStream& operator>>(QDataStream& in, TextBlock& blk);			// Input
	friend QDataStream& operator<<(QDataStream& out, const TextBlock& blk);		// Output

private:

	TextBlockID _blockId;
	QTextBlockFormat _blockFormat;
	TextListID _listId;
	QVector<qint32> _fPosBegin;
	QVector<qint32> _fPosEnd;

public:

	TextBlock();		// Empty constructor, to use with deserialization

	TextBlock(qint32 blockNum, qint32 authorId, QTextBlockFormat fmt);
	TextBlock(TextBlockID blockId, QTextBlockFormat fmt);

	/* setters */
	void setFormat(QTextBlockFormat fmt);
	void setList(TextListID id);
	void setBegin(QVector<qint32> fPosBegin);
	void setEnd(QVector<qint32> fPosEnd);

	/* getters */
	TextBlockID getId() const;
	QTextBlockFormat getFormat() const;
	TextListID getListId() const;
	QVector<qint32> begin() const;
	QVector<qint32> end() const;
	bool isEmpty() const;

};

Q_DECLARE_METATYPE(TextBlock);



/************* TEXTLIST CLASS *************/

class TextList
{
	/* Operators for QDataStream serialization and deserialization */
	friend QDataStream& operator>>(QDataStream& in, TextList& lst);				// Input
	friend QDataStream& operator<<(QDataStream& out, const TextList& lst);		// Output

private:

	TextListID _listId;
	QTextListFormat _listFormat;
	QList<TextBlockID> _blocks;

public:

	TextList();			// Empty constructor, to use with deserialization

	TextList(qint32 listNum, qint32 authorId, QTextListFormat fmt);
	TextList(TextListID listId, QTextListFormat fmt);

	void addBlock(TextBlockID id);
	void removeBlock(TextBlockID id);
	void setFormat(QTextListFormat fmt);

	/* getters */
	TextListID getId() const;
	QTextListFormat getFormat() const;
	QList<TextBlockID> getBlocks() const;
	bool isEmpty() const;

};

Q_DECLARE_METATYPE(TextList);

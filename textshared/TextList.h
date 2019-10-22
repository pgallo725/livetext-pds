#pragma once

#include <QTextListFormat>

class TextBlockID;
class TextListID;
class TextList;


class TextListID
{
	/* Operators for QDataStream serialization and deserialization */
	friend QDataStream& operator>>(QDataStream& in, TextListID& lid);			// Input
	friend QDataStream& operator<<(QDataStream& out, const TextListID& lid);	// Output
	
private:

	qint32 _number;
	qint32 _authorId;

public:

	TextListID();				// Empty constructor for deserialization purposes

	TextListID(qint32 num, qint32 uid);		// Construct a TextListID with the specified pair of values
	TextListID(const TextList& block);		// Copy the TextListID of the reference block
	TextListID(std::nullptr_t);				// Construct a null TextListID

	qint32 getListNumber() const;
	qint32 getAuthorId() const;

	// Comparators to use this type as a key in QMap collections, or store in a QList
	bool operator<(const TextListID& other) const noexcept;
	bool operator>(const TextListID& other) const noexcept;
	bool operator==(const TextListID& other) const noexcept;
	bool operator!=(const TextListID& other) const noexcept;
	operator bool() const noexcept;
};

Q_DECLARE_METATYPE(TextListID);


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
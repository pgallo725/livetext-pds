#pragma once
#ifndef _TEXT_UTILS_
#define _TEXT_UTILS_

#include <QDataStream>
#include <QVector>
#include <QMetaType>

class Position
{
	/* Operators for QDataStream serialization and deserialization */
	friend QDataStream& operator>>(QDataStream& in, Position& sym);				// Input
	friend QDataStream& operator<<(QDataStream& out, const Position& sym);		// Output

private:

	QVector<qint32> posIndexes;

public:

	Position();		// empty fractional position (only for deserialization)

	Position(QVector<qint32> values);

	/* getters */
	qint32 size() const;
	qint32 operator[](int index) const;
	qint32 getAuthorId() const;

	/* comparison operators */
	bool operator==(const Position& other) const;
	bool operator<(const Position& other) const;
	bool operator>(const Position& other) const;
};

Q_DECLARE_METATYPE(Position);


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
#endif

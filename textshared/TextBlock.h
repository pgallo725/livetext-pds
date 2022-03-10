#pragma once
#ifndef _TEXT_BLOCK_
#define _TEXT_BLOCK_

#include <QTextBlockFormat>
#include "TextUtils.h"


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
	Position _fPosBegin;
	Position _fPosEnd;

public:

	TextBlock();		// Empty constructor, to use with deserialization

	TextBlock(qint32 blockNum, qint32 authorId, QTextBlockFormat fmt);
	TextBlock(TextBlockID blockId, QTextBlockFormat fmt);

	/* setters */
	void setFormat(QTextBlockFormat fmt);
	void setList(TextListID id);
	void setBegin(Position fPosBegin);
	void setEnd(Position fPosEnd);

	/* getters */
	TextBlockID getId() const;
	QTextBlockFormat getFormat() const;
	TextListID getListId() const;
	const Position& begin() const;
	const Position& end() const;
	bool isEmpty() const;

};

Q_DECLARE_METATYPE(TextBlock);

#endif

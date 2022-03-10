#pragma once
#ifndef _TEXT_LIST_
#define _TEXT_LIST_

#include <QTextListFormat>
#include "TextUtils.h"


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

#endif

#include "TextList.h"


/*************** TEXTLIST CLASS ***************/


TextList::TextList()
	: _listId(TextListID(nullptr))
{
}

TextList::TextList(qint32 listNum, qint32 authorId, QTextListFormat fmt)
	: _listId(TextListID(listNum, authorId)), _listFormat(fmt)
{
}

TextList::TextList(TextListID listId, QTextListFormat fmt)
	: _listId(listId), _listFormat(fmt)
{
}


void TextList::addBlock(TextBlockID id)
{
	_blocks.append(id);
}

void TextList::removeBlock(TextBlockID id)
{
	_blocks.removeAll(id);
}

void TextList::setFormat(QTextListFormat fmt)
{
	_listFormat = fmt;
}


TextListID TextList::getId() const
{
	return _listId;
}

QTextListFormat TextList::getFormat() const
{
	return _listFormat;
}

QList<TextBlockID> TextList::getBlocks() const
{
	// List of *unordered* blocks
	return _blocks;
}

bool TextList::isEmpty() const
{
	return _blocks.count() == 0;
}


/*************** SERIALIZATION OPERATORS ***************/


// TextList deserialization operator
QDataStream& operator>>(QDataStream& in, TextList& lst)
{
	in >> lst._listId >> lst._listFormat >> lst._blocks;

	return in;
}

// TextList serialization operator
QDataStream& operator<<(QDataStream& out, const TextList& lst)
{
	out << lst._listId << lst._listFormat << lst._blocks;

	return out;
}
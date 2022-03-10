#pragma once
#ifndef _TEXT_EDIT_MESSAGE_
#define _TEXT_EDIT_MESSAGE_

#include "Message.h"
#include "Symbol.h"
#include "TextBlock.h"
#include "TextList.h"


class CharsInsertMessage : public Message
{
	friend MessageFactory;

private:

	QVector<Symbol> m_symbols;
	TextBlockID m_blockId;
	QTextBlockFormat m_blockFmt;
	bool m_flag;

protected:

	CharsInsertMessage();	// empty constructor

	// Constructor for CharsInsert messages, carrying the list of symbols in a block and its format
	CharsInsertMessage(QVector<Symbol> symbols, bool isLast, TextBlockID bId, QTextBlockFormat blkFmt);

	void writeTo(QDataStream& stream) const override;
	void readFrom(QDataStream& stream) override;

public:

	~CharsInsertMessage() {};

	QVector<Symbol> getSymbols() const;
	TextBlockID getBlockId() const;
	QTextBlockFormat getBlockFormat() const;
	bool getIsLast() const;
}; 


class CharsDeleteMessage : public Message
{
	friend MessageFactory;

private:

	QVector<Position> m_fPositions;

protected:

	CharsDeleteMessage();	// empty constructor

	// Constructor for CharsDelete messages, with the fractional positions to delete
	CharsDeleteMessage(QVector<Position> positions);

	void writeTo(QDataStream& stream) const override;
	void readFrom(QDataStream& stream) override;

public:

	~CharsDeleteMessage() {};

	QVector<Position> getPositions() const;
};


class CharsFormatMessage : public Message
{
	friend MessageFactory;

private:

	QVector<Position> m_fPos;
	QVector<QTextCharFormat> m_charFmt;

protected:

	CharsFormatMessage();	// empty constructor

	// Constructor for CharsFormat messages, with the fractional positions and the new symbol formats
	CharsFormatMessage(QVector<Position> positions, QVector<QTextCharFormat> fmts);

	void writeTo(QDataStream& stream) const override;
	void readFrom(QDataStream& stream) override;

public:

	~CharsFormatMessage() {};

	QVector<Position> getPositions() const;
	QVector<QTextCharFormat> getCharFormats() const;
};


class BlockEditMessage : public Message
{
	friend MessageFactory;

private:

	TextBlockID m_blockId;
	QTextBlockFormat m_blockFmt;

protected:

	BlockEditMessage();		// empty constructor

	// Constructor for BlockEdit messages
	BlockEditMessage(TextBlockID blockId, QTextBlockFormat fmt);

	void writeTo(QDataStream& stream) const override;
	void readFrom(QDataStream& stream) override;

public:

	~BlockEditMessage() {};

	TextBlockID getBlockId() const;
	QTextBlockFormat getBlockFormat() const;
};


class ListEditMessage : public Message
{
	friend MessageFactory;

private:

	TextBlockID m_blockId;
	TextListID m_listId;
	QTextListFormat m_listFmt;

protected:

	ListEditMessage();		// empty constructor

	// Constructor for ListEdit messages, used for add/remove/create
	ListEditMessage(TextBlockID blockId, TextListID listId, QTextListFormat fmt);

	void writeTo(QDataStream& stream) const override;
	void readFrom(QDataStream& stream) override;

public:

	~ListEditMessage() {};

	TextBlockID getBlockId() const;
	TextListID getListId() const;
	QTextListFormat getListFormat() const;
};
#endif // _TEXT_EDIT_MESSAGE_

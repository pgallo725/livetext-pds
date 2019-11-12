#pragma once

#include "Message.h"
#include "Symbol.h"
#include "TextBlock.h"
#include "TextList.h"


class CharInsertMessage : public Message
{
	friend MessageFactory;

private:

	Symbol m_symbol;
	bool m_flag;

protected:

	CharInsertMessage();	// empty constructor

	// Constructor for CharInsert messages, carrying the symbol object
	CharInsertMessage(Symbol symbol, bool isLast);

	void writeTo(QDataStream& stream) const override;
	void readFrom(QDataStream& stream) override;

public:

	~CharInsertMessage() {};

	Symbol getSymbol() const;
	bool getIsLast() const;
};


class CharDeleteMessage : public Message
{
	friend MessageFactory;

private:

	Position m_fPos;

protected:

	CharDeleteMessage();	// empty constructor

	// Constructor for CharDelete messages, with the fractional position of the symbol to delete
	CharDeleteMessage(Position position);

	void writeTo(QDataStream& stream) const override;
	void readFrom(QDataStream& stream) override;

public:

	~CharDeleteMessage() {};

	Position getPosition() const;
};


class CharFormatMessage : public Message
{
	friend MessageFactory;

private:

	Position m_fPos;
	QTextCharFormat m_charFmt;

protected:

	CharFormatMessage();	// empty constructor

	// Constructor for CharFormat messages, with the fractional position and the new format of the symbol
	CharFormatMessage(Position position, QTextCharFormat fmt);

	void writeTo(QDataStream& stream) const override;
	void readFrom(QDataStream& stream) override;

public:

	~CharFormatMessage() {};

	Position getPosition() const;
	QTextCharFormat getCharFormat() const;
};


class BulkInsertMessage : public Message
{
	friend MessageFactory;

private:

	QList<Symbol> m_symbols;
	TextBlockID m_blockId;
	QTextBlockFormat m_blockFmt;
	bool m_flag;
	
protected:

	BulkInsertMessage();	// empty constructor

	// Constructor for BulkInsert messages, carrying the list of symbols in a block and its format
	BulkInsertMessage(QList<Symbol> symbols, bool isLast, TextBlockID bId, QTextBlockFormat blkFmt);

	void writeTo(QDataStream& stream) const override;
	void readFrom(QDataStream& stream) override;

public:

	~BulkInsertMessage() {};

	QList<Symbol> getSymbols() const;
	TextBlockID getBlockId() const;
	QTextBlockFormat getBlockFormat() const;
	bool getIsLast() const;
};


class BulkDeleteMessage : public Message
{
	friend MessageFactory;

private:

	QList<Position> m_fPositions;

protected:

	BulkDeleteMessage();	// empty constructor

	// Constructor for BulkDelete messages, with the fractional positions to delete
	BulkDeleteMessage(QList<Position> positions);

	void writeTo(QDataStream& stream) const override;
	void readFrom(QDataStream& stream) override;

public:

	~BulkDeleteMessage() {};

	QList<Position> getPositions() const;
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
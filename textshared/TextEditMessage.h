#pragma once

#include "Message.h"
#include "Symbol.h"


class CharInsertMessage : public Message
{
	friend MessageFactory;

private:

	Symbol m_symbol;

protected:

	CharInsertMessage();	// empty constructor

	// Constructor for CharInsert messages, carrying the symbol object
	CharInsertMessage(Symbol symbol);

	void writeTo(QDataStream& stream) const override;
	void readFrom(QDataStream& stream) override;

public:

	~CharInsertMessage() {};

	Symbol& getSymbol();
};


class CharDeleteMessage : public Message
{
	friend MessageFactory;

private:

	QVector<qint32> m_fPos;

protected:

	CharDeleteMessage();	// empty constructor

	// Constructor for CharDelete messages, with the fractional position of the symbol to delete
	CharDeleteMessage(QVector<qint32> position);

	void writeTo(QDataStream& stream) const override;
	void readFrom(QDataStream& stream) override;

public:

	~CharDeleteMessage() {};

	QVector<qint32> getPosition() const;
};


class CharFormatMessage : public Message
{
	friend MessageFactory;

private:

	QVector<qint32> m_fPos;
	QTextCharFormat m_charFmt;

protected:

	CharFormatMessage();	// empty constructor

	// Constructor for CharFormat messages, with the fractional position and the new format of the symbol
	CharFormatMessage(QVector<qint32> position, QTextCharFormat fmt);

	void writeTo(QDataStream& stream) const override;
	void readFrom(QDataStream& stream) override;

public:

	~CharFormatMessage() {};

	QVector<qint32> getPosition() const;
	QTextCharFormat getCharFormat() const;
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
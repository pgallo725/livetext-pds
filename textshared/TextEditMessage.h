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

	void writeContent(QDataStream& stream) const override;

public:

	~CharInsertMessage() {};

	void readFrom(QDataStream& stream) override;

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

	void writeContent(QDataStream& stream) const override;

public:

	~CharDeleteMessage() {};

	void readFrom(QDataStream& stream) override;

	QVector<qint32> getPosition() const;
};


class BlockEditMessage : public Message
{
	friend MessageFactory;

private:

	qint32 m_editorId;
	QPair<qint32, qint32> m_blockId;
	QTextBlockFormat m_blockFmt;

protected:

	BlockEditMessage();		// empty constructor

	// Constructor for BlockEdit messages, editorId is useful to identify the author of the change
	BlockEditMessage(QPair<qint32, qint32> blockId, QTextBlockFormat fmt, qint32 editorId);

	void writeContent(QDataStream& stream) const override;

public:

	~BlockEditMessage() {};

	void readFrom(QDataStream& stream) override;

	qint32 getAuthorId() const;
	QPair<qint32, qint32> getBlockIdPair() const;
	QTextBlockFormat getBlockFormat() const;
};

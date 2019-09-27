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

	// Constructor for CharInsert messages
	CharInsertMessage(Symbol symbol);

public:

	~CharInsertMessage() {};

	void readFrom(QDataStream& stream) override;
	void sendTo(QSslSocket* socket) const override;

	Symbol& getSymbol();
};


class CharDeleteMessage : public Message
{
	friend MessageFactory;

private:

	QVector<qint32> m_fPos;

protected:

	CharDeleteMessage();	// empty constructor

	// Constructor for CharDelete messages
	CharDeleteMessage(QVector<qint32> position);

public:

	~CharDeleteMessage() {};

	void readFrom(QDataStream& stream) override;
	void sendTo(QSslSocket* socket) const override;

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

public:

	~BlockEditMessage() {};

	void readFrom(QDataStream& stream) override;
	void sendTo(QSslSocket* socket) const override;

	qint32 getAuthorId() const;
	QPair<qint32, qint32> getBlockIdPair() const;
	QTextBlockFormat getBlockFormat() const;
};

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
	void sendTo(QTcpSocket* socket) const override;

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
	void sendTo(QTcpSocket* socket) const override;

	QVector<qint32> getPosition() const;
};

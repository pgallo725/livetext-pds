#pragma once

#include "Message.h"

#include "Symbol.h"
#include <QVector>


class TextEditMessage : public Message
{
private:

	Symbol m_symbol;
	QVector<qint32> m_position;

public:

	// Build an empty TextEditMessage, to be filled later reading data from a socket stream
	TextEditMessage(MessageType m);

	// Constructor for CharInsert messages
	TextEditMessage(MessageType charInsert, Symbol symbol);

	// Constructor for CharDelete messages
	TextEditMessage(MessageType charDelete, QVector<qint32> position);

	~TextEditMessage() {};


	void readFrom(QDataStream& stream) override;
	void sendTo(QTcpSocket* socket) override;


	/* getters */
	Symbol& getSymbol();
	QVector<qint32> getPosition();

};

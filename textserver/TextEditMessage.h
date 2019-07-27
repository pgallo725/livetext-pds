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

	TextEditMessage(MessageType m, QDataStream& streamIn);
	~TextEditMessage();

	Symbol& getSymbol();
	QVector<qint32> getPosition();
};

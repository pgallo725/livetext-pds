#include "TextEditMessage.h"

#include <QDataStream>


TextEditMessage::TextEditMessage(MessageType m, QDataStream& streamIn) : Message(m)
{
	switch (m)
	{
	case CharInsert:
		streamIn >> m_symbol;
		break;

	case CharDelete:
		streamIn >> m_position;
	default:
		break;
	}
}

TextEditMessage::~TextEditMessage()
{
}

Symbol& TextEditMessage::getSymbol()
{
	return m_symbol;
}

QVector<qint32> TextEditMessage::getPosition()
{
	if (m_type == CharInsert)
		return m_symbol._fPos;
	else return m_position;
}

#include "TextEditMessage.h"


TextEditMessage::TextEditMessage(MessageType m) 
	: Message(m)
{
}

TextEditMessage::TextEditMessage(MessageType charInsert, Symbol symbol)
	: Message(charInsert), m_symbol(symbol)
{
}

TextEditMessage::TextEditMessage(MessageType charDelete, QVector<qint32> position)
	: Message(charDelete), m_position(position)
{
}


void TextEditMessage::readFrom(QDataStream& stream)
{
	switch (m_type)
	{
		case CharInsert:
			stream >> m_symbol;
			break;

		case CharDelete:
			stream >> m_position;
			break;

		default:
			// throw ?
			break;
	}
}

void TextEditMessage::sendTo(QTcpSocket* socket)
{
	QDataStream streamOut(socket);

	streamOut << (quint16)m_type;

	switch (m_type)
	{
		case CharInsert:
			streamOut << m_symbol;
			break;

		case CharDelete:
			streamOut << m_position;
			break;

		default:
			// throw ?
			break;
	}
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

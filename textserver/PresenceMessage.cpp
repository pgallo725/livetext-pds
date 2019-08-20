#include "PresenceMessage.h"


PresenceMessage::PresenceMessage(MessageType m)
	: Message(m), m_userId(-1), m_cursorPos(-1)
{
}

PresenceMessage::PresenceMessage(MessageType moveCursor, qint32 id, qint32 newPosition)
	: Message(moveCursor), m_userId(id), m_cursorPos(newPosition)
{
}

PresenceMessage::PresenceMessage(MessageType newOrUpdatePresence, qint32 id, QString username, QImage icon)
	: Message(newOrUpdatePresence), m_userId(id), m_userName(username), m_userIcon(icon), m_cursorPos(-1)
{
}

PresenceMessage::PresenceMessage(MessageType removePresence, qint32 id)
	: Message(removePresence), m_userId(id), m_cursorPos(-1)
{
}
	

void PresenceMessage::readFrom(QDataStream& stream)
{
	stream >> m_userId;

	switch (m_type)
	{
	case MoveCursor:
		stream >> m_cursorPos;
		break;

	case UserAccountUpdate:
	case AddUserPresence:
		stream >> m_userName >> m_userIcon;
		break;

	case RemoveUserPresence:
		break;

	default:
		// throw ?
		break;
	}
}

void PresenceMessage::sendTo(QTcpSocket* socket)
{
	QDataStream streamOut(socket);

	streamOut << (quint16)m_type;

	switch (m_type)
	{
	case MoveCursor:
		streamOut << m_cursorPos;
		break;

	case UserAccountUpdate:
	case AddUserPresence:
		streamOut << m_userName << m_userIcon;
		break;

	case RemoveUserPresence:
		break;

	default:
		// throw ?
		break;
	}
}



qint32 PresenceMessage::getUserId() const
{
	return m_userId;
}

QString PresenceMessage::getNickname() const
{
	return m_userName;
}

QImage PresenceMessage::getIcon() const
{
	return m_userIcon;
}

qint32 PresenceMessage::getCursorPosition() const
{
	return m_cursorPos;
}

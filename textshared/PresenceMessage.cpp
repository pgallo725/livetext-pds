#include "PresenceMessage.h"


/*************** CURSOR MOVEMENT MESSAGE ***************/

CursorMoveMessage::CursorMoveMessage()
	: Message(CursorMove), m_userId(-1)
{
}

CursorMoveMessage::CursorMoveMessage(qint32 userId, qint32 newPosition)
	: Message(CursorMove), m_userId(userId), m_cursorPos(newPosition)
{
}

void CursorMoveMessage::readFrom(QDataStream& stream)
{
	stream >> m_userId >> m_cursorPos;
}

void CursorMoveMessage::sendTo(QTcpSocket* socket) const
{
	QDataStream streamOut(socket);
	QByteArray bufferData;
	QDataStream streamBuffer(&bufferData, QIODevice::WriteOnly);

	streamBuffer << m_userId << m_cursorPos;

	streamOut << (quint16)CursorMove << bufferData.size() << bufferData;

	//streamOut << (quint16)CursorMove << m_userId << m_cursorPos;
}

qint32 CursorMoveMessage::getUserId() const
{
	return m_userId;
}

qint32 CursorMoveMessage::getCursorPosition() const
{
	return m_cursorPos;
}


/*************** PRESENCE UPDATE MESSAGE ***************/

PresenceUpdateMessage::PresenceUpdateMessage()
	: Message(PresenceUpdate), m_userId(-1)
{
}

PresenceUpdateMessage::PresenceUpdateMessage(qint32 userId, QString nickname, QImage icon)
	: Message(PresenceUpdate), m_userId(userId), m_userName(nickname), m_userIcon(icon)
{
}

void PresenceUpdateMessage::readFrom(QDataStream& stream)
{
	stream >> m_userId >> m_userName >> m_userIcon;
}

void PresenceUpdateMessage::sendTo(QTcpSocket* socket) const
{
	QDataStream streamOut(socket);
	QByteArray bufferData;
	QDataStream streamBuffer(&bufferData, QIODevice::WriteOnly);

	streamBuffer << m_userId << m_userName << m_userIcon;

	streamOut << (quint16)PresenceUpdate << bufferData.size() << bufferData;
	//streamOut << (quint16)PresenceUpdate << m_userId << m_userName << m_userIcon;
}

qint32 PresenceUpdateMessage::getUserId() const
{
	return m_userId;
}

QString PresenceUpdateMessage::getNickname() const
{
	return m_userName;
}

QImage PresenceUpdateMessage::getIcon() const
{
	return m_userIcon;
}


/*************** PRESENCE ADD MESSAGE ***************/

PresenceAddMessage::PresenceAddMessage()
	: Message(PresenceAdd), m_userId(-1)
{
}

PresenceAddMessage::PresenceAddMessage(qint32 userId, QString nickname, QImage icon)
	: Message(PresenceAdd), m_userId(userId), m_userName(nickname), m_userIcon(icon)
{
}

void PresenceAddMessage::readFrom(QDataStream& stream)
{
	stream >> m_userId >> m_userName >> m_userIcon;
}

void PresenceAddMessage::sendTo(QTcpSocket* socket) const
{
	QDataStream streamOut(socket);
	QByteArray bufferData;
	QDataStream streamBuffer(&bufferData, QIODevice::WriteOnly);

	streamBuffer << m_userId << m_userName << m_userIcon;

	streamOut << (quint16)PresenceAdd << bufferData.size() << bufferData;
	//streamOut << (quint16)PresenceAdd << m_userId << m_userName << m_userIcon;
}

qint32 PresenceAddMessage::getUserId() const
{
	return m_userId;
}

QString PresenceAddMessage::getNickname() const
{
	return m_userName;
}

QImage PresenceAddMessage::getIcon() const
{
	return m_userIcon;
}


/*************** PRESENCE REMOVE MESSAGE ***************/

PresenceRemoveMessage::PresenceRemoveMessage()
	: Message(PresenceRemove), m_userId(-1)
{
}

PresenceRemoveMessage::PresenceRemoveMessage(qint32 userId)
	: Message(PresenceRemove), m_userId(userId)
{
}

void PresenceRemoveMessage::readFrom(QDataStream& stream)
{
	stream >> m_userId;
}

void PresenceRemoveMessage::sendTo(QTcpSocket* socket) const
{
	QDataStream streamOut(socket);
	QByteArray bufferData;
	QDataStream streamBuffer(&bufferData, QIODevice::WriteOnly);

	streamBuffer << m_userId;

	streamOut << (quint16)PresenceRemove << bufferData.size() << bufferData;
	//streamOut << (quint16)PresenceRemove << m_userId;
}

qint32 PresenceRemoveMessage::getUserId() const
{
	return m_userId;
}

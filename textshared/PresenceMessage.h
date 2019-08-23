#pragma once

#include "Message.h"
#include <QImage>


class CursorMoveMessage : public Message
{
	friend MessageFactory;

private:

	qint32 m_userId;
	qint32 m_cursorPos;

protected:

	CursorMoveMessage();	// empty constructor

	// Construct a CursorMove message, specifying the new position of the user's cursor
	CursorMoveMessage(qint32 userId, qint32 newPosition);

public:

	~CursorMoveMessage() {};

	void readFrom(QDataStream& stream) override;
	void sendTo(QTcpSocket* socket) const override;

	qint32 getUserId() const;
	qint32 getCursorPosition() const;
};


class PresenceUpdateMessage : public Message
{
	friend MessageFactory;

private:

	qint32 m_userId;
	QString m_userName;
	QImage m_userIcon;

protected:

	PresenceUpdateMessage();	// empty constructor

	// Construct a PresenceUpdate message, specifying the updated user's data
	PresenceUpdateMessage(qint32 userId, QString nickname, QImage icon);

public:

	~PresenceUpdateMessage() {};

	void readFrom(QDataStream& stream) override;
	void sendTo(QTcpSocket* socket) const override;

	qint32 getUserId() const;
	QString getNickname() const;
	QImage getIcon() const;
};


class PresenceAddMessage : public Message
{
	friend MessageFactory;

private:

	qint32 m_userId;
	QString m_userName;
	QImage m_userIcon;

protected:

	PresenceAddMessage();	// empty constructor

	// Construct a PresenceAdd message, specifying the new user's data
	PresenceAddMessage(qint32 userId, QString nickname, QImage icon);

public:

	~PresenceAddMessage() {};

	void readFrom(QDataStream& stream) override;
	void sendTo(QTcpSocket* socket) const override;

	qint32 getUserId() const;
	QString getNickname() const;
	QImage getIcon() const;
};


class PresenceRemoveMessage : public Message
{
	friend MessageFactory;

private:

	qint32 m_userId;

protected:

	PresenceRemoveMessage();	// empty constructor

	// Construct a PresenceRemove message, specifying the id of the user which disconnected
	PresenceRemoveMessage(qint32 userId);

public:

	~PresenceRemoveMessage() {};

	void readFrom(QDataStream& stream) override;
	void sendTo(QTcpSocket* socket) const override;

	qint32 getUserId() const;
};
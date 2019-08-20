#pragma once

#include "Message.h"
#include "User.h"


class PresenceMessage : public Message
{
private:

	qint32 m_userId;
	QString m_userName;
	QImage m_userIcon;
	qint32 m_cursorPos;
	

public:

	// Build an empty PresenceMessage, to be filled later by reading data from a socket stream
	PresenceMessage(MessageType m);

	// Construct a MoveCursor message, specifying the new position of the user's cursor
	PresenceMessage(MessageType moveCursor, qint32 id, qint32 newPosition);

	// Construct an AddUserPresence or UserAccountUpdate message, specifying the new or updated user's data
	PresenceMessage(MessageType newOrUpdatePresence, qint32 id, QString nickname, QImage icon);

	// Construct a RemoveUserPresence message, specifying the id of the user which disconnected
	PresenceMessage(MessageType removePresence, qint32 id);

	~PresenceMessage() {};


	void readFrom(QDataStream& stream) override;
	void sendTo(QTcpSocket* socket) override;


	/* getters */
	qint32 getUserId() const;
	QString getNickname() const;
	QImage getIcon() const;
	qint32 getCursorPosition() const;
};

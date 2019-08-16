#pragma once

#include "Message.h"


class PresenceMessage :
	public Message
{
private:

	// fields

public:

	// Build an empty PresenceMessage, to be filled later by reading data from a socket stream
	PresenceMessage(MessageType m);

	// Other constructors

	~PresenceMessage() {};


	void readFrom(QDataStream& stream) override;
	void sendTo(QTcpSocket* socket) override;


	/* getters */
};

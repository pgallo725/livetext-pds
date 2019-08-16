#include "PresenceMessage.h"


PresenceMessage::PresenceMessage(MessageType m)
	: Message(m)
{
}


void PresenceMessage::readFrom(QDataStream& stream)
{
}

void PresenceMessage::sendTo(QTcpSocket* socket)
{
}

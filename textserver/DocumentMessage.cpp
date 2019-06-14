#include "DocumentMessage.h"

DocumentMessage::DocumentMessage(MessageType m, QDataStream& streamIn) :
	Message(m)
{
}

DocumentMessage::~DocumentMessage()
{
}

#include "AccountMessage.h"

AccountMessage::AccountMessage(MessageType m, QDataStream& streamIn) : Message(m)
{
	streamIn >> user;
}

AccountMessage::~AccountMessage()
{
}

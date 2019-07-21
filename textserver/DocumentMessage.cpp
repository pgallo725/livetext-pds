#include "DocumentMessage.h"

DocumentMessage::DocumentMessage(MessageType m, QDataStream& streamIn, QString username) :
	Message(m), creatorName(username)
{
	streamIn >> docName;
	URI = username + "/" + docName + "/" + "deadbeef"; /* TODO: random sequence */
}

DocumentMessage::~DocumentMessage()
{
}

QString DocumentMessage::getDocName()
{
	return docName;
}

QString DocumentMessage::getURI()
{
	return URI;
}

QString DocumentMessage::getUserName()
{
	return creatorName;
}

#include "DocumentMessage.h"
#include "ServerException.h"

DocumentMessage::DocumentMessage(MessageType m, QDataStream& streamIn, QString username) :
	Message(m), userName(username)
{
	switch (m) {
	case NewDocument:
		streamIn >> docName;
		URI = username + "/" + docName + "/" + "deadbeef"; /* TODO: random sequence */
		break;
	case OpenDocument: 
		streamIn >> URI;
		break;
	default:
		throw MessageUnknownTypeException(m);
		break;
	}
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
	return userName;
}

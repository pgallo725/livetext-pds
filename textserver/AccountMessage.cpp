#include "AccountMessage.h"
#include "ServerException.h"

AccountMessage::AccountMessage(MessageType m, QDataStream& streamIn) : Message(m), user(User())
{
	quint16 typeOfMessage;

	switch (m) {
	case AccountCreate:
		streamIn >> user;
		break;
	case AccountUpDate:
		break;
	default:
		throw MessageUnknownTypeException(typeOfMessage);
		break;
	}
}

AccountMessage::~AccountMessage()
{
}

QString AccountMessage::getUserName()
{
	return user.getUsername();
}

QString AccountMessage::getNickname()
{
	return user.getNickName();
}

QString AccountMessage::getPasswd()
{
	return user.getPassword();
}

QPixmap AccountMessage::getIcon()
{
	return user.getIcon();
}

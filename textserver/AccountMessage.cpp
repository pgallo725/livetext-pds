#include "AccountMessage.h"

AccountMessage::AccountMessage(MessageType m, QDataStream& streamIn) : Message(m), user(User())
{
	switch (m) {
	case AccountCreate:
		streamIn >> user;
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

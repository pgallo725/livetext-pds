#include "AccountMessage.h"
#include "ServerException.h"

AccountMessage::AccountMessage(MessageType m, QDataStream& streamIn) : 
	Message(m), user(User()), fieldType(-1)
{
	switch (m) {
	case AccountCreate:
		streamIn >> user;
		break;
	case AccountUpDate:
		streamIn >> fieldType;
		switch (fieldType) {
		case ChangeNickname:
		case ChangeIcon:
		case ChangePassword:
			streamIn >> field;
			break;

		case RemoveNickname:
		case RemoveIcon:
			field = QVariant();
			break;

		default:
			throw FieldWrongException(fieldType);
			break;
		}
		break;

	default:
		throw MessageUnknownTypeException(m);
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

QImage AccountMessage::getIcon()
{
	return user.getIcon();
}

quint16 AccountMessage::getFieldType()
{
	return fieldType;
}

QVariant AccountMessage::getField()
{
	return field;
}

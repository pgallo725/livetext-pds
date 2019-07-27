#pragma once

#include <QVariant>

#include "Message.h"
#include "User.h"

class AccountMessage :
	public Message
{
private:
	User user;
	quint16 field;
	QVariant newField;
public:
	AccountMessage(MessageType m, QDataStream& streamIn);
	~AccountMessage();

	QString getUserName();
	QString getNickname();
	QString getPasswd();
	QPixmap getIcon();
	quint16 getFiledType();
	QVariant getFiled();
};

enum fieldType {
	changeNickname,
	removeNickname,
	changeIcon,
	removeIcon,
	changePassword
};
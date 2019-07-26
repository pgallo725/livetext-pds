#pragma once

#include "Message.h"
#include "User.h"

class AccountMessage :
	public Message
{
private:
	User user;
	quint16 field;
	QString newField;
	QPixmap newIcon;
public:
	AccountMessage(MessageType m, QDataStream& streamIn);
	~AccountMessage();

	QString getUserName();
	QString getNickname();
	QString getPasswd();
	QPixmap getIcon();
	quint16 getFiledType();
	QString getField();
	QPixmap getNewIcon();
};

enum fieldType {
	changeNickname,
	removeNickname,
	changeIcon,
	removeIcon,
	changePassword
};
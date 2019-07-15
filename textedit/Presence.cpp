#include "Presence.h"

Presence::Presence(QString name, QString nickname, QColor color, QPixmap profilePic, QTextEdit* textedit) : _name(name), _nickname(nickname), _profilePicture(profilePic)
{
	_color = color;
	_userCursor = new QTextCursor(textedit->document());
	_label = new QLabel(textedit);
}

QColor Presence::color()
{
	return _color;
}

QString Presence::name()
{
	return _name;
}

QString Presence::nickname()
{
	return _nickname;
}

QPixmap Presence::profilePicture()
{
	return _profilePicture;
}

QTextCursor* Presence::cursor()
{
	return _userCursor;
}

QLabel* Presence::label()
{
	return _label;
}

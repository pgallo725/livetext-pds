#include "Presence.h"

Presence::Presence(QString name, QColor color, QPixmap profilePic, QTextEdit* textedit, QString nickname) : _name(name), _nickname(nickname), _profilePicture(profilePic)
{
	_color = color;
	_userCursor = new QTextCursor(textedit->document());
	_label = new QLabel(textedit);
}

Presence::~Presence()
{

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

QList<QTextEdit::ExtraSelection> Presence::userText()
{
	return _userText;
}

void Presence::addUserText(QTextEdit::ExtraSelection selection)
{
	_userText.append(selection);
}

void Presence::clearSelections()
{
	_userText.clear();
}

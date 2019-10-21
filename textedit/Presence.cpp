#include "Presence.h"

#include <QPainter>

Presence::Presence(QString name, QColor color, QPixmap profilePic, QTextEdit* textedit, QString nickname) : _name(name), _nickname(nickname), _color(color)
{
	_userCursor = new QTextCursor(textedit->document());
	_label = new QLabel(textedit);

	//Setting up user icon
	QPixmap background(32, 32);
	background.fill(_color);

	//Setting-up background
	QPainter painter(&background);

	//Build user-icon with colored frame
	painter.drawPixmap(3, 3, 26, 26, profilePic.scaled(26, 26, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	painter.end();

	_profilePicture = background;
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

QAction* Presence::actionHighlightText()
{
	return _iconPressedAction;
}

void Presence::addUserText(QTextEdit::ExtraSelection selection)
{
	_userText.append(selection);
}

void Presence::clearSelections()
{
	_userText.clear();
}

void Presence::setAction(QAction* action)
{
	_iconPressedAction = action;
}

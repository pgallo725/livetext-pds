#include "Presence.h"

#include <QPainter>

Presence::Presence(QString name, QColor color, QPixmap profilePic, QTextEdit* textedit, QString nickname) : _name(name), _nickname(nickname), _color(color)
{
	_userCursor = new QTextCursor(textedit->document());
	_label = new QLabel(textedit);

	//Setting up background to draw a rectangle
	QPixmap background(32, 32);

	//Setting-up painter
	QPainter painter(&background);
	painter.setRenderHint(QPainter::Antialiasing);
	painter.setRenderHint(QPainter::HighQualityAntialiasing, true);
	painter.setRenderHint(QPainter::SmoothPixmapTransform, true);

	//Set up painter's pen
	QPen pen(Qt::NoBrush, 6);
	pen.setColor(_color);
	painter.setPen(pen);

	//Draw rectangle on background
	painter.drawRect(0, 0, 32, 32);

	//Build user-icon with colored frame
	painter.drawPixmap(4, 4, 24, 24, profilePic.scaled(24, 24, Qt::KeepAspectRatio, Qt::SmoothTransformation));
	painter.end();

	_profilePicture = background;
}

Presence::~Presence()
{
	//Nothing to do
}

/************************ GETTERS ************************/
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


/************************ TEXT HIGHLIGHTING ************************/
void Presence::addUserText(QTextEdit::ExtraSelection selection)
{
	_userText.append(selection);
}

void Presence::clearSelections()
{
	//Clears all highlighted text
	_userText.clear();
}

void Presence::setAction(QAction* action)
{
	//Editor action
	_iconPressedAction = action;
}

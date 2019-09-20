#ifndef PRESENCE_H
#define PRESENCE_H

#include <QString>
#include <QColor>
#include <QPixmap>
#include <QLabel>
#include <QList>
#include <QTextEdit>
#include <QTextCursor>
#include <QTextDocument>

class Presence
{
private:
	QString _name;
	QString _nickname;
	QColor _color;
	QPixmap _profilePicture;
	QLabel* _label;
	QTextCursor* _userCursor;
	QList<QTextEdit::ExtraSelection> _userText;
public:
	Presence(QString name, QColor color, QPixmap profilePic, QTextEdit* textedit, QString nickname = "");
	~Presence();

	QColor color();
	QString name();
	QString nickname();
	QPixmap profilePicture();
	QTextCursor* cursor();
	QLabel* label();

};



#endif // PRESENCE_H
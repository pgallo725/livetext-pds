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
	//User parameter
	QString _name;
	QString _nickname;
	QColor _color;
	QPixmap _profilePicture;
	QLabel* _label;
	QTextCursor* _userCursor;
	QAction* _iconPressedAction;

public:
	Presence(QString name, QColor color, QPixmap profilePic, QTextEdit* textedit, QString nickname = "");
	~Presence();

	//Getters
	QColor color();
	QString name();
	QString nickname();
	QPixmap profilePicture();
	QTextCursor* cursor();
	QLabel* label();
	QAction* actionHighlightText();
	
	//Text highlighting
	void setAction(QAction* action);
};



#endif // PRESENCE_H
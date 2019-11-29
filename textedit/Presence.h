#ifndef PRESENCE_H
#define PRESENCE_H

#include <QString>
#include <QColor>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QTextEdit>
#include <QTextCursor>

class Presence
{
private:
	//User parameter
	QString _name;
	QColor _color;
	QPixmap _profilePicture;
	QLabel* _label;
	QTextCursor* _userCursor;
	QAction* _iconPressedAction;
	qint32 _id;

public:
	Presence(qint32 id, QString name, QColor color, QImage profilePic, QTextEdit* textedit);
	~Presence();

	//Getters
	QColor color();
	QString name();
	QPixmap profilePicture();
	QTextCursor* cursor();
	QLabel* label();
	QAction* actionHighlightText();
	qint32 id();
	
	//Text highlighting
	void setAction(QAction* action);
};



#endif // PRESENCE_H
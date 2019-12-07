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
	qint32 _id;
	QString _name;
	QColor _color;
	QPixmap _profilePicture;
	QAction* _iconPressedAction;
	QTextCursor* _userCursor;
	QLabel* _label;

public:
	Presence(qint32 id, QString name, QColor color, QImage profilePic, QTextEdit* textedit);
	~Presence();

	//Getters
	qint32 id() const;
	QString name() const;
	QColor color() const;
	QPixmap profilePicture() const;
	QAction* actionHighlightText() const;
	QTextCursor* cursor() const;
	QLabel* label() const;
	

	//Text highlighting
	void setAction(QAction* action);
};



#endif // PRESENCE_H
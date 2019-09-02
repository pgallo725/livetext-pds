#ifndef DOCUMENTEDITOR_H
#define DOCUMENTEDITOR_H

#include <QObject>
#include <Document.h>
#include <User.h>
#include "textedit.h"

class DocumentEditor : public QObject
{
	Q_OBJECT

public:
	DocumentEditor(Document doc, TextEdit* editor, User user, QObject* parent = nullptr);
	void openDocument();

private:
	Document _document;
	User _user;
	TextEdit* _textedit;

private slots:
	void deleteCharAtIndex(int position);
	void addCharAtIndex(QChar ch, QTextCharFormat fmt, int position);

signals:
	void deleteChar(QVector<qint32> fPos);
	void insertChar(Symbol s);
};


#endif //DOCUMENTEDITOR_H
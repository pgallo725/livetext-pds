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

public slots:
	void addSymbol(Symbol s);
	void removeSymbol(QVector<int> position);
	void applyBlockFormat(QPair<int, int> blockId, QTextBlockFormat fmt, qint32 userId);


private:
	Document _document;
	User _user;
	TextEdit* _textedit;

private slots:
	void deleteCharAtIndex(int position);
	void addCharAtIndex(QChar ch, QTextCharFormat fmt, int position);
	void generateExtraSelection();
	void changeBlockFormat(qint32 userId, int position, QTextBlockFormat fmt);


signals:
	void deleteChar(QVector<qint32> fPos);
	void insertChar(Symbol s);
	void blockFormatChanged(QPair<int, int> blockId, QTextBlockFormat fmt, qint32 userId);
};


#endif //DOCUMENTEDITOR_H
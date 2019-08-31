#ifndef DOCUMENTEDITOR_H
#define DOCUMENTEDITOR_H

#include <QObject>
#include <Document.h>
#include "textedit.h"

class DocumentEditor : public QObject
{
Q_OBJECT

public:
	DocumentEditor(Document doc, TextEdit* editor, QObject* parent = nullptr);
	void openDocument();

private:
	Document _document;
	TextEdit* _textedit;

private slots:
	void deleteCharAtIndex(int position);

signals:
	void deleteChar(QVector<qint32> fPos);
};


#endif //DOCUMENTEDITOR_H
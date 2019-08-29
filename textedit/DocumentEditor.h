#ifndef DOCUMENTEDITOR_H
#define DOCUMENTEDITOR_H

#include <QObject>
#include <Document.h>
#include "textedit.h"

class DocumentEditor : public QObject
{
public:
	DocumentEditor(Document doc, TextEdit* editor, QObject* parent = nullptr);
	void openDocument();

private:
	Document _document;
	TextEdit* _textedit;

};


#endif //DOCUMENTEDITOR_H
#ifndef DOCUMENTEDITOR_H
#define DOCUMENTEDITOR_H

#include <QObject>
#include <Document.h>

class DocumentEditor : public QObject
{
public:
	DocumentEditor(Document doc, QObject* parent = nullptr);


private:
	Document _document;
};


#endif //DOCUMENTEDITOR_H
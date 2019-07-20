#include "Document.h"

Document::Document(QString name, QString uri, QString userName):
	docName(name), URI(uri), creatorName(userName)
{
}

Document::~Document()
{
}

QString Document::getURI()
{
	return URI;
}

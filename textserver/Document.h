#pragma once

#include <QString>

class Document
{
private:
	QString docName;
	int docId;
	// TODO: document structur

public:
	Document(QString name, int id);
	~Document();
};


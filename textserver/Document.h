#pragma once

#include <QObject>
#include <QString>

class Document
{
private:
	QString docName;
	QString URI;
	QString creatorName;
	// TODO: document structur

public:
	Document(QString name, QString uri, QString userName);
	~Document();

	/* getter */
	QString getURI();
};


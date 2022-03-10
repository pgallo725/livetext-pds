#pragma once
#ifndef __SERVER_DATABASE__
#define __SERVER_DATABASE__


#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <User.h>

class ServerDatabase
{
private:

	// Prepared queries
	QSqlQuery qInsertNewUser;
	QSqlQuery qUpdateUser;
	QSqlQuery qInsertDocEditor;
	QSqlQuery qRemoveDocEditor;
	QSqlQuery qRemoveDoc;
	QSqlQuery qCountDocEditors;
	QSqlQuery qSelectUserDocs;

public:

	// Neutral ctor and dtor; the initialization work is done inside open()

	ServerDatabase() {};

	~ServerDatabase() {};

	// Database connection and initialization
	// the database file is created if it doesn't exist
	void open(QString dbName);

	// Queries

	void insertUser(const User& user);
	void updateUser(const User& user);
	void addDocToUser(QString username, QString uri);
	void removeDocFromUser(QString username, QString uri);
	void removeDoc(QString uri);

	int getMaxUserID();
	QList<User> readUsersList();
	QStringList readUserDocuments(QString username);
	QStringList readDocumentURIs();
	int countDocEditors(QString docURI);

};
#endif // __SERVER_DATABASE__

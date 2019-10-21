#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QImage>
#include <QStringList>
#include "User.h"

class ServerDatabase
{
private:

	QSqlQuery qInsertNewUser;
	QSqlQuery qUpdateUser;
	QSqlQuery qInsertNewDocToUser;
	QSqlQuery qRemoveDocFromUser;
	QSqlQuery qRemoveDoc;
	QSqlQuery qSelectDocuments;
	QSqlQuery qCountDocumentEditors;
	QSqlQuery qSelectMaxUserID;
	QSqlQuery qSelectDocsUsers;

public:

	ServerDatabase();

	~ServerDatabase();

	// Database connection and initialization

	void initialize(QString dbName);

	// Queries

	void insertUser(User user, QString username, int userId, QString nickname, QByteArray passhash, QByteArray salt, QByteArray icon);
	void updateUser(QString username, QString nickname, QByteArray passhash, QByteArray salt, QByteArray icon);
	void addDocToUser(QString username, QString uri);
	void removeDocFromUser(QString username, QString uri);
	void removeDoc(QString uri);

	int getMaxUserID();
	QList<User> readUsersList();
	QStringList readUserDocuments(QString username);
	QStringList readDocumentURIs();
	int countDocEditors(QString docURI);

};
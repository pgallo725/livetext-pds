#pragma once

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QImage>
#include "User.h"

class ServerDatabase
{
private:

	QSqlQuery qInsertNewUser;
	QSqlQuery qUpdateUser;
	QSqlQuery qInsertNewDocToUser;
	QSqlQuery qRemoveDocFromUser;
	QSqlQuery qSelectDocuments;
	QSqlQuery qCountDocumentEditors;
	QSqlQuery qSelectMaxUserID;

public:

	ServerDatabase();

	~ServerDatabase();

	// Database connection and initialization

	void initialize(QString dbName);

	// Queries

	bool insertUser(User user, QString username, int userId, QString nickname, QByteArray passhash, QByteArray salt, QByteArray icon);
	bool updateUser(QString username, QString nickname, QByteArray passhash, QByteArray salt, QByteArray icon);
	bool addDocToUser(QString username, QString uri);
	bool removeDocFromUser(QString username, QString uri);

	int getMaxUserID();
	QList<QString> readDocumentURIs();
	int countDocEditors(QString docURI);

};
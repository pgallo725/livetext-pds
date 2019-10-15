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

public:

	ServerDatabase();

	~ServerDatabase();

	void initialized(QString dbName);

	// queries exec

	bool inserNewUser(User user, QString username, int userId, QString nickname, QByteArray passhash, QByteArray salt, QByteArray icon);
	bool updateUser(QString username, QString nickname, QByteArray passhash, QByteArray salt, QByteArray icon);
	bool insertNewDocToUser(QString username, QString uri);
	bool removeNewDocFromUser(QString username, QString uri);

	// connect
	// queries
};
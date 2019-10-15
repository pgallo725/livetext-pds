#include "ServerDatabase.h"
#include "ServerException.h"

#include <QString>
#include <QVariant>
#include <QByteArray>
#include <QBuffer>

ServerDatabase::ServerDatabase()
{
}

ServerDatabase::~ServerDatabase()
{
}

void ServerDatabase::initialized(QString dbName)
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	db.setDatabaseName(dbName);
	bool ok = db.open();
	if (!ok)
	{
		throw StartupException("Impossible to connect to the database");
	}

	qInsertNewUser = QSqlQuery(db);
	qUpdateUser = QSqlQuery(db);
	qInsertNewDocToUser = QSqlQuery(db);
	qRemoveDocFromUser = QSqlQuery(db);

	qInsertNewUser.prepare("INSERT INTO Users (Username, UserID, Nickname, PassHash, Salt, Icon) "
		"VALUES (:username, :id, :nickname, :passhash, :salt, :icon)");

	qUpdateUser.prepare("UPDATE Users SET Nickname = :nickname, PassHash = :passhash, Salt = :salt, Icon = :icon "
		"WHERE Username = :username");

	qInsertNewDocToUser.prepare("INSERT INTO DocEditors (Username, DocURI) VALUES (:username, :uri)");

	qRemoveDocFromUser.prepare("DELETE FROM DocEditors WHERE Username = :username AND DocURI = :uri");
}

bool ServerDatabase::inserNewUser(User user, QString username, int userId, QString nickname, QByteArray passhash, QByteArray salt, QByteArray icon)
{
	qInsertNewUser.bindValue(":username", username);
	qInsertNewUser.bindValue(":id", userId);
	qInsertNewUser.bindValue(":nickname", nickname);
	qInsertNewUser.bindValue(":passhash", passhash);
	qInsertNewUser.bindValue(":salt", salt);
	qInsertNewUser.bindValue(":icon", icon);

	return qInsertNewUser.exec();
}

bool ServerDatabase::updateUser(QString username, QString nickname, QByteArray passhash, QByteArray salt, QByteArray icon)
{
	qUpdateUser.bindValue(":username", username);
	qUpdateUser.bindValue(":nickname", nickname);
	qUpdateUser.bindValue(":passhash", passhash);
	qUpdateUser.bindValue(":salt", salt);
	qUpdateUser.bindValue(":icon", icon);

	return qUpdateUser.exec();
}

bool ServerDatabase::insertNewDocToUser(QString username, QString uri)
{
	qInsertNewDocToUser.bindValue(":username", username);
	qInsertNewDocToUser.bindValue(":uri", uri);

	return qInsertNewDocToUser.exec();
}

bool ServerDatabase::removeNewDocFromUser(QString username, QString uri)
{
	qRemoveDocFromUser.bindValue(":username", username);
	qRemoveDocFromUser.bindValue(":uri", uri);

	return qRemoveDocFromUser.exec();
}

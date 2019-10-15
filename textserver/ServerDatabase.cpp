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

void ServerDatabase::initialize(QString dbName)
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
	qSelectDocuments = QSqlQuery(db);
	qCountDocumentEditors = QSqlQuery(db);
	qSelectMaxUserID = QSqlQuery(db);

	qInsertNewUser.prepare("INSERT INTO Users (Username, UserID, Nickname, PassHash, Salt, Icon) "
		"VALUES (:username, :id, :nickname, :passhash, :salt, :icon)");

	qUpdateUser.prepare("UPDATE Users SET Nickname = :nickname, PassHash = :passhash, Salt = :salt, Icon = :icon "
		"WHERE Username = :username");

	qInsertNewDocToUser.prepare("INSERT INTO DocEditors (Username, DocURI) VALUES (:username, :uri)");

	qRemoveDocFromUser.prepare("DELETE FROM DocEditors WHERE Username = :username AND DocURI = :uri");

	qSelectDocuments.prepare("SELECT DISTINCT DocURI FROM DocEditors");

	qCountDocumentEditors.prepare("SELECT COUNT(*) FROM DocEditors WHERE DocURI = :uri");

	qSelectMaxUserID.prepare("SELECT MAX(UserID) FROM Users");
}

bool ServerDatabase::insertUser(User user, QString username, int userId, QString nickname, QByteArray passhash, QByteArray salt, QByteArray icon)
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

bool ServerDatabase::addDocToUser(QString username, QString uri)
{
	qInsertNewDocToUser.bindValue(":username", username);
	qInsertNewDocToUser.bindValue(":uri", uri);

	return qInsertNewDocToUser.exec();
}

bool ServerDatabase::removeDocFromUser(QString username, QString uri)
{
	qRemoveDocFromUser.bindValue(":username", username);
	qRemoveDocFromUser.bindValue(":uri", uri);

	return qRemoveDocFromUser.exec();
}


int ServerDatabase::getMaxUserID()
{
	if (qSelectMaxUserID.exec() && qSelectMaxUserID.isActive())
	{
		qSelectMaxUserID.next();
		if (qSelectMaxUserID.isValid())
			return qSelectMaxUserID.value(0).toInt() + 1;
	}
	else 
	{
		throw StartupException("Can't read users IDs from the database");
	}

	return 0;
}

QList<QString> ServerDatabase::readDocumentURIs()
{
	QList<QString> documents;

	if (qSelectDocuments.exec("SELECT DISTINCT DocURI FROM DocEditors") && qSelectDocuments.isActive())
	{
		// Load all the document URIs in a QString list
		qSelectDocuments.next();
		while (qSelectDocuments.isValid())
		{
			documents.append(qSelectDocuments.value(0).toString());
			qSelectDocuments.next();
		}
	}
	else
	{
		throw StartupException("Can't access documents table in database");
	}

	return documents;
}

int ServerDatabase::countDocEditors(QString docURI)
{
	qCountDocumentEditors.bindValue(":uri", docURI);

	if (qCountDocumentEditors.exec() && qCountDocumentEditors.isActive())
	{
		qCountDocumentEditors.next();
		return qCountDocumentEditors.value(0).toInt();
	}
	else
	{
		throw StartupException("Can't access documents table in database");
	}
}

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
	qRemoveDoc = QSqlQuery(db);
	qSelectDocuments = QSqlQuery(db);
	qCountDocumentEditors = QSqlQuery(db);
	qSelectMaxUserID = QSqlQuery(db);
	qSelectDocsUsers = QSqlQuery(db);

	qInsertNewUser.prepare("INSERT INTO Users (Username, UserID, Nickname, PassHash, Salt, Icon) "
		"VALUES (:username, :id, :nickname, :passhash, :salt, :icon)");

	qUpdateUser.prepare("UPDATE Users SET Nickname = :nickname, PassHash = :passhash, Salt = :salt, Icon = :icon "
		"WHERE Username = :username");

	qInsertNewDocToUser.prepare("INSERT INTO DocEditors (Username, DocURI) VALUES (:username, :uri)");

	qRemoveDocFromUser.prepare("DELETE FROM DocEditors WHERE Username = :username AND DocURI = :uri");

	qRemoveDoc.prepare("DELETE FROM DocEditors WHERE DocURI = :uri");

	qSelectDocuments.prepare("SELECT DISTINCT DocURI FROM DocEditors");

	qCountDocumentEditors.prepare("SELECT COUNT(*) FROM DocEditors WHERE DocURI = :uri");

	qSelectMaxUserID.prepare("SELECT MAX(UserID) FROM Users");

	qSelectDocsUsers.prepare("SELECT DocURI FROM DocEditors WHERE Username = :username");
}

void ServerDatabase::insertUser(User user, QString username, int userId, QString nickname, QByteArray passhash, QByteArray salt, QByteArray icon)
{
	qInsertNewUser.bindValue(":username", username);
	qInsertNewUser.bindValue(":id", userId);
	qInsertNewUser.bindValue(":nickname", nickname);
	qInsertNewUser.bindValue(":passhash", passhash);
	qInsertNewUser.bindValue(":salt", salt);
	qInsertNewUser.bindValue(":icon", icon);

	if (!qInsertNewUser.exec())
		throw DataBaseWriteRecordException(qInsertNewUser.lastQuery().toStdString(), qInsertNewUser.lastError());
}

void ServerDatabase::updateUser(QString username, QString nickname, QByteArray passhash, QByteArray salt, QByteArray icon)
{
	qUpdateUser.bindValue(":username", username);
	qUpdateUser.bindValue(":nickname", nickname);
	qUpdateUser.bindValue(":passhash", passhash);
	qUpdateUser.bindValue(":salt", salt);
	qUpdateUser.bindValue(":icon", icon);

	if (!qUpdateUser.exec())
		throw DataBaseWriteRecordException(qUpdateUser.lastQuery().toStdString(), qUpdateUser.lastError());
}

void ServerDatabase::addDocToUser(QString username, QString uri)
{
	qInsertNewDocToUser.bindValue(":username", username);
	qInsertNewDocToUser.bindValue(":uri", uri);

	if (!qInsertNewDocToUser.exec())
		throw DataBaseWriteRecordException(qInsertNewDocToUser.lastQuery().toStdString(), qInsertNewDocToUser.lastError());
}

void ServerDatabase::removeDocFromUser(QString username, QString uri)
{
	qRemoveDocFromUser.bindValue(":username", username);
	qRemoveDocFromUser.bindValue(":uri", uri);

	if (!qRemoveDocFromUser.exec())
		throw DataBaseWriteRecordException(qRemoveDocFromUser.lastQuery().toStdString(), qRemoveDocFromUser.lastError());
}

void ServerDatabase::removeDoc(QString uri)
{
	qRemoveDoc.bindValue(":uri", uri);

	if (!qRemoveDoc.exec())
		throw DataBaseWriteRecordException(qRemoveDoc.lastQuery().toStdString(), qRemoveDoc.lastError());
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
		throw DataBaseReadTableException(qSelectMaxUserID.lastQuery().toStdString(), qSelectMaxUserID.lastError());
	}

	return 0;
}

QList<User> ServerDatabase::readUsersList()
{
	QList<User> users;
	QSqlQuery query;
	if (query.exec("SELECT * FROM Users") && query.isActive()) {
		// Read all the users' information from the database and load them in memory
		query.next();
		while (query.isValid())
		{
			User user(query.value("Username").toString(),
				query.value("UserID").toInt(),
				query.value("Nickname").toString(),
				query.value("PassHash").toByteArray(),
				query.value("Salt").toByteArray(),
				QImage::fromData(query.value("Icon").toByteArray()));

			users.append(user);

			query.next();
		}
	}
	else {
		throw DataBaseReadTableException(query.lastQuery().toStdString(), query.lastError());
	}

	return users;
}

QStringList ServerDatabase::readUserDocuments(QString username)
{
	QStringList docs;
	qSelectDocsUsers.bindValue(":username", username);
	
	if (qSelectDocsUsers.exec() && qSelectDocsUsers.isActive())
	{
		qSelectDocsUsers.next();
		while (qSelectDocsUsers.isValid())
		{
			docs << qSelectDocsUsers.value(0).toString();
			qSelectDocsUsers.next();
		}
	}
	else
	{
		throw DataBaseReadTableException(qSelectDocsUsers.lastQuery().toStdString(), qSelectDocsUsers.lastError());
	}

	return docs;
}

QStringList ServerDatabase::readDocumentURIs()
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
		throw DataBaseReadTableException(qSelectDocuments.lastQuery().toStdString(), qSelectDocuments.lastError());
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
		throw DataBaseReadTableException(qCountDocumentEditors.lastQuery().toStdString(), qCountDocumentEditors.lastError());
	}
}

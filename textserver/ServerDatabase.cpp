#include "ServerDatabase.h"
#include "ServerException.h"

#include <QFile>
#include <QVariant>
#include <QImage>
#include <QByteArray>
#include <QBuffer>


void ServerDatabase::open(QString dbName)
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
	bool creatingDb = !QFile::exists(dbName);
	db.setDatabaseName(dbName);
	if (!db.open())
	{
		throw DatabaseConnectionException(db.lastError());
	}
	if (creatingDb)
	{
		// Database initialization queries

		QSqlQuery createTableUsers(db);								// Query to create the Users table
		if (!createTableUsers.exec("CREATE TABLE \"Users\" (\
			\"Username\"	TEXT,\
			\"UserID\"	INTEGER NOT NULL UNIQUE,\
			\"Nickname\"	TEXT,\
			\"PassHash\"	BLOB NOT NULL,\
			\"Salt\"	BLOB NOT NULL,\
			\"Icon\"	BLOB,\
			PRIMARY KEY(\"Username\"))"))
		{
			throw DatabaseCreateException(createTableUsers.lastQuery().toStdString(), createTableUsers.lastError());
		}

		QSqlQuery createTableDocs(db);								  // Query to create the DocEditors table
		if (!createTableDocs.exec("CREATE TABLE \"DocEditors\" (\
			\"Username\"	TEXT NOT NULL,\
			\"DocURI\"	TEXT NOT NULL,\
			FOREIGN KEY(\"Username\") REFERENCES \"Users\"(\"Username\") ON UPDATE CASCADE ON DELETE CASCADE,\
			PRIMARY KEY(\"Username\", \"DocURI\"))"))
		{
			throw DatabaseCreateException(createTableDocs.lastQuery().toStdString(), createTableDocs.lastError());
		}
	}

	// Prepare all var-arg queries, leave only parameter binding for later

	qInsertNewUser	 = QSqlQuery(db);
	qUpdateUser		 = QSqlQuery(db);
	qInsertDocEditor = QSqlQuery(db);
	qRemoveDocEditor = QSqlQuery(db);
	qRemoveDoc		 = QSqlQuery(db);
	qCountDocEditors = QSqlQuery(db);
	qSelectUserDocs	 = QSqlQuery(db);

	// Insertion query of a new record in the Users table
	qInsertNewUser.prepare("INSERT INTO Users (Username, UserID, Nickname, PassHash, Salt, Icon) "
		"VALUES (:username, :id, :nickname, :passhash, :salt, :icon)");

	// Update query of an existing record in the Users table
	qUpdateUser.prepare("UPDATE Users SET Nickname = :nickname, PassHash = :passhash, Salt = :salt, Icon = :icon "
		"WHERE Username = :username");

	// Insertion query of a new User-URI pair in the DocEditors table
	qInsertDocEditor.prepare("INSERT INTO DocEditors (Username, DocURI) VALUES (:username, :uri)");

	// Deletion query of a User-URI pair from the DocEditors table
	qRemoveDocEditor.prepare("DELETE FROM DocEditors WHERE Username = :username AND DocURI = :uri");

	// Deletion query of all records referring to a document URI in the DocEditors table
	qRemoveDoc.prepare("DELETE FROM DocEditors WHERE DocURI = :uri");

	// Query to count the editors of a document (DocEditors table)
	qCountDocEditors.prepare("SELECT COUNT(*) FROM DocEditors WHERE DocURI = :uri");

	// Selection query of all the URIs owned by a user (DocEditors table)
	qSelectUserDocs.prepare("SELECT DocURI FROM DocEditors WHERE Username = :username");
}

void ServerDatabase::insertUser(const User& user)
{
	QByteArray icon;
	QBuffer buffer(&icon);
	buffer.open(QIODevice::WriteOnly);
	user.getIcon().save(&buffer, "PNG");	// writes image into the bytearray in PNG format

	qInsertNewUser.bindValue(":username", user.getUsername());
	qInsertNewUser.bindValue(":id", user.getUserId());
	qInsertNewUser.bindValue(":nickname", user.getNickname());
	qInsertNewUser.bindValue(":passhash", user.getPasswordHash());
	qInsertNewUser.bindValue(":salt", user.getSalt());
	qInsertNewUser.bindValue(":icon", icon);

	if (!qInsertNewUser.exec())
		throw DatabaseWriteException(qInsertNewUser.lastQuery().toStdString(), qInsertNewUser.lastError());
}

void ServerDatabase::updateUser(const User& user)
{
	QByteArray icon;
	QBuffer buffer(&icon);
	buffer.open(QIODevice::WriteOnly);
	user.getIcon().save(&buffer, "PNG");	// writes image into the bytearray in PNG format

	qUpdateUser.bindValue(":username", user.getUsername());
	qUpdateUser.bindValue(":nickname", user.getNickname());
	qUpdateUser.bindValue(":passhash", user.getPasswordHash());
	qUpdateUser.bindValue(":salt", user.getSalt());
	qUpdateUser.bindValue(":icon", icon);

	if (!qUpdateUser.exec())
		throw DatabaseWriteException(qUpdateUser.lastQuery().toStdString(), qUpdateUser.lastError());
}

void ServerDatabase::addDocToUser(QString username, QString uri)
{
	qInsertDocEditor.bindValue(":username", username);
	qInsertDocEditor.bindValue(":uri", uri);

	if (!qInsertDocEditor.exec())
		throw DatabaseWriteException(qInsertDocEditor.lastQuery().toStdString(), qInsertDocEditor.lastError());
}

void ServerDatabase::removeDocFromUser(QString username, QString uri)
{
	qRemoveDocEditor.bindValue(":username", username);
	qRemoveDocEditor.bindValue(":uri", uri);

	if (!qRemoveDocEditor.exec())
		throw DatabaseWriteException(qRemoveDocEditor.lastQuery().toStdString(), qRemoveDocEditor.lastError());
}

void ServerDatabase::removeDoc(QString uri)
{
	qRemoveDoc.bindValue(":uri", uri);

	if (!qRemoveDoc.exec())
		throw DatabaseWriteException(qRemoveDoc.lastQuery().toStdString(), qRemoveDoc.lastError());
}


int ServerDatabase::getMaxUserID()
{
	QSqlQuery query;
	if (query.exec("SELECT MAX(UserID) FROM Users") && query.isActive())
	{
		// Get the max value of the UserID column and return it incremented by 1 (first available ID)
		query.next();
		if (query.isValid())
			return query.value(0).toInt() + 1;
	}
	else 
	{
		throw DatabaseReadException(query.lastQuery().toStdString(), query.lastError());
	}

	return 0;
}

QList<User> ServerDatabase::readUsersList()
{
	QList<User> users;
	QSqlQuery query;
	if (query.exec("SELECT * FROM Users") && query.isActive()) 
	{
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
	else 
	{
		throw DatabaseReadException(query.lastQuery().toStdString(), query.lastError());
	}

	return users;
}

QStringList ServerDatabase::readUserDocuments(QString username)
{
	QStringList docs;
	qSelectUserDocs.bindValue(":username", username);
	
	if (qSelectUserDocs.exec() && qSelectUserDocs.isActive())
	{
		qSelectUserDocs.next();
		while (qSelectUserDocs.isValid())
		{
			docs << qSelectUserDocs.value(0).toString();
			qSelectUserDocs.next();
		}
	}
	else
	{
		throw DatabaseReadException(qSelectUserDocs.lastQuery().toStdString(), qSelectUserDocs.lastError());
	}

	return docs;
}

QStringList ServerDatabase::readDocumentURIs()
{
	QList<QString> documents;
	QSqlQuery query;
	if (query.exec("SELECT DISTINCT DocURI FROM DocEditors") && query.isActive())
	{
		// Load all the document URIs in a QString list
		query.next();
		while (query.isValid())
		{
			documents.append(query.value(0).toString());
			query.next();
		}
	}
	else
	{
		throw DatabaseReadException(query.lastQuery().toStdString(), query.lastError());
	}

	return documents;
}

int ServerDatabase::countDocEditors(QString docURI)
{
	qCountDocEditors.bindValue(":uri", docURI);

	if (qCountDocEditors.exec() && qCountDocEditors.isActive())
	{
		qCountDocEditors.next();
		return qCountDocEditors.value(0).toInt();
	}
	else
	{
		throw DatabaseReadException(qCountDocEditors.lastQuery().toStdString(), qCountDocEditors.lastError());
	}
}

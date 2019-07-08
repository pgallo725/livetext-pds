#pragma once

//#include <QObject>
#include <QPixmap>


class User
{
	/* Operators for QDataStream serialization and deserialization */
	friend QDataStream& operator>>(QDataStream& in, User& user);			// Input
	friend QDataStream& operator<<(QDataStream& out, const User& user);		// Output

private:

	QString m_username;
	QString m_nickname;
	QString m_passwd;		// hashed

	QPixmap m_icon;		// nullable, check with QPixmap::isNull()

	QList<QString> m_documents;

public:

	User() { };	 // Use this to construct an empty user and populate the fields later

	User(QString username, QString nickname, QString passwd);
	User(QString username, QString nickname, QString passwd, QPixmap icon);

	~User();


	/* getter methods */
	QString getInfo();
	QString getUsername();
	QString getPassword();

	/* setter methods */
	void addDocument(QString docUri);

	
	
};


QDataStream& operator>>(QDataStream& in, User& user);			// Input deserialization
QDataStream& operator<<(QDataStream& out, const User& user);	// Output serialization



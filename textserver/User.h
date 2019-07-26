#pragma once

//#include <QObject>
#include <QPixmap>
#include <QSet>

class User
{
	/* Operators for QDataStream serialization and deserialization */
	friend QDataStream& operator>>(QDataStream& in, User& user);			// Input
	friend QDataStream& operator<<(QDataStream& out, const User& user);		// Output

private:

	QString m_username;
	int m_userId;
	QString m_nickname;
	QString m_passwd;		// hashed
	QPixmap m_icon;		// nullable, check with QPixmap::isNull()
	QSet<QString> m_documents;

public:

	User() { };	 // Use this to construct an empty user and populate the fields later

	User(QString username, int userId, QString nickname, QString passwd);
	User(QString username, int userId, QString nickname, QString passwd, QPixmap icon);

	~User();

	/* getter methods */
	QString getInfo();
	QString getUsername();
	int getUserId();
	QString getNickName();
	QString getPassword();
	QPixmap getIcon();

	/* setter methods */
	void addDocument(QString docUri);
	void setNickname(QString newNickname);
	void deleteNickname();
	void setIcon(QPixmap newIcon);
	void deleteIcon();
	void changePassword(QString newPassword);
};


QDataStream& operator>>(QDataStream& in, User& user);			// Input deserialization
QDataStream& operator<<(QDataStream& out, const User& user);	// Output serialization



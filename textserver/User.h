#pragma once

//#include <QObject>
#include <QPixmap>
#include <QSet>
#include <QStringList>

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
	QStringList m_documents;

public:

	User() { };	 // Use this to construct an empty user and populate the fields later

	//User(QString username, int userId, QString nickname, QString passwd);
	User(QString username, int userId, QString nickname, QString passwd, QPixmap icon = QPixmap());

	~User();

	/* getter methods */
	QString getInfo();
	QString getUsername();
	int getUserId();
	QString getNickName();
	QString getPassword();
	QPixmap getIcon();
	QStringList getDocuments();

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



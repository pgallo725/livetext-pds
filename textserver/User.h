#pragma once

#include <QObject>
#include <QImage>
#include <QSet>
#include <QStringList>
//#include <QMutex>
//#include <QMutexLocker>

class User //: public QObject
{
	//Q_OBJECT
	/* Operators for QDataStream serialization and deserialization */
	friend QDataStream& operator>>(QDataStream& in, User& user);			// Input
	friend QDataStream& operator<<(QDataStream& out, const User& user);		// Output
	
private:

	QString m_username;
	int m_userId;
	QString m_nickname;
	QString m_passwd;		// hashed
	QImage m_icon;		// nullable, check with QPixmap::isNull()
	QStringList m_documents;
	//QMutex m;
public:

	User();	 // Use this to construct an empty user and populate the fields later

	User(QString username, int userId, QString nickname, QString passwd, QImage icon = QImage());

	~User();

	/* getter methods */
	QString getUsername();
	int getUserId();
	QString getNickName();
	QString getPassword();
	QImage getIcon();
	QStringList getDocuments();

	/* setter methods */
	void addDocument(QString docUri);
	void setNickname(QString newNickname);
	void deleteNickname();
	void setIcon(QImage newIcon);
	void deleteIcon();
	void changePassword(QString newPassword);

	void removeDocument(QString uri);
};


QDataStream& operator>>(QDataStream& in, User& user);			// Input deserialization
QDataStream& operator<<(QDataStream& out, const User& user);	// Output serialization



#pragma once

#include <QImage>
#include <Document.h>
#include <QList>


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
	QImage m_icon;			// nullable, check with QImage::isNull()
	QList<URI> m_documents;

public:

	User();	 // Use this to construct an empty user and populate the fields later

	User(QString username, int userId, QString nickname, QString passwd, QImage icon = QImage());

	~User();

	/* getter methods */
	QString getUsername();
	int getUserId();
	QString getNickname();
	QString getPassword();
	QImage getIcon();
	QList<URI> getDocuments();

	/* setter methods */
	void addDocument(URI docUri);
	void setNickname(QString newNickname);
	void deleteNickname();
	void setIcon(QImage newIcon);
	void setId(int id);
	void deleteIcon();
	void changePassword(QString newPassword);

	void removeDocument(URI uri);
};


QDataStream& operator>>(QDataStream& in, User& user);			// Input deserialization
QDataStream& operator<<(QDataStream& out, const User& user);	// Output serialization


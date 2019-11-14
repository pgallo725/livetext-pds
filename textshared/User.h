#pragma once

#include <QImage>
#include <QList>
#include <QByteArray>
#include "Document.h"

#define MAX_NAME_LENGTH	  50			  // 50 characters
#define MAX_IMAGE_SIZE	  1*1024*1024	  // 1 MB


class User
{
	/* Operators for QDataStream serialization and deserialization */
	friend QDataStream& operator>>(QDataStream& in, User& user);			// Input deserialization
	friend QDataStream& operator<<(QDataStream& out, const User& user);		// Output serialization
	
private:

	QString m_username;
	int m_userId;
	QString m_nickname;
	QByteArray m_passwd;		// hashed
	QByteArray m_salt;			// randomly generated
	QImage m_icon;				// nullable, check with QImage::isNull()
	QList<URI> m_documents;

	static const QString saltCharacters;

public:

	User();	 // Use this to construct an empty user and populate the fields later

	User(QString username, int userId, QString nickname, QString passwd, QImage icon = QImage());
	User(QString username, int userId, QString nickname, QByteArray passhash, QByteArray salt, QImage icon);

	~User();

	/* getters */
	int getUserId() const;
	QString getUsername() const;
	QString getNickname() const;
	QByteArray getPasswordHash() const;
	QByteArray getSalt() const;
	QImage getIcon() const;
	QList<URI> getDocuments() const;
	bool hasDocument(URI uri) const;
	URI getURIat(int index) const;

	/* setters */
	void addDocument(URI docUri);
	void removeDocument(URI uri);
	void setNickname(QString newNickname);
	void setIcon(QImage newIcon);
	void setPassword(QString newPassword);
	void update(QString nickname, QImage icon, QString password);
	void rollback(const User& backup);
};

#include "User.h"

#include <QString>
#include <QDataStream>


/*
User::User(QString username, int userId, QString nickname, QString passwd)
	: m_username(username), m_userId(userId), m_nickname(nickname),
	m_passwd(passwd), m_icon(QPixmap()), m_documents(QStringList())
{
}*/

User::User() : m_userId(-1)
{
}

User::User(QString username, int userId, QString nickname, QString passwd, QImage icon)
	: m_username(username), m_userId(userId), m_nickname(nickname),
	m_passwd(passwd), m_icon(icon), m_documents(QStringList())
{
}

User::~User()
{
	//NOTHIG TO DO
}

QString User::getUsername()
{
	return m_username;
}

int User::getUserId()
{
	return m_userId;
}

QString User::getNickname()
{
	return m_nickname;
}

QString User::getPassword()
{
	return m_passwd;
}

QImage User::getIcon()
{
	return m_icon;
}

QStringList User::getDocuments()
{
	return m_documents;
}


void User::addDocument(QString docUri)
{
	if (!m_documents.contains(docUri))
		m_documents << docUri;
}

void User::setNickname(QString newNickname)
{
	m_nickname = newNickname;
}

void User::deleteNickname()
{
	m_nickname = m_username;
}

void User::setIcon(QImage newIcon)
{
	m_icon = newIcon;
}

void User::deleteIcon()
{
	m_icon = QImage();
}

void User::setId(int id) {

	m_userId = id;

}

void User::changePassword(QString newPassword)
{
	//TODO: decrypt
	m_passwd = newPassword;
}

void User::removeDocument(QString uri)
{
	m_documents.removeOne(uri);
}



QDataStream& operator>>(QDataStream& in, User& user)
{
	// Object deserialized reading field by field from the stream

	in >> user.m_username >> user.m_nickname
		>> user.m_passwd
		>> user.m_icon
		>> user.m_documents;

	return in;
}

QDataStream& operator<<(QDataStream& out, const User& user)
{
	// Object serialized as the sequence of its member fields

	out << user.m_username << user.m_nickname
		<< user.m_passwd
		<< user.m_icon
		<< user.m_documents;

	return out;
}

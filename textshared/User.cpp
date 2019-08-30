#include "User.h"

#include <QDataStream>


User::User() : m_userId(-1)
{
}

User::User(QString username, int userId, QString nickname, QString passwd, QImage icon)
	: m_username(username), m_userId(userId), m_nickname(nickname),
	m_passwd(passwd), m_icon(icon)
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

bool User::hasDocument(URI uri)
{
	return m_documents.contains(uri);
}

QList<URI> User::getDocuments()
{
	return m_documents;
}

URI User::getURIat(int index)
{
	return m_documents.at(index);
}


void User::addDocument(URI docUri)
{
	m_documents << docUri;
}

void User::removeDocument(URI uri)
{
	m_documents.removeOne(uri);
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

void User::changePassword(QString newPassword)
{
	//TODO: decrypt
	m_passwd = newPassword;
}



QDataStream& operator>>(QDataStream& in, User& user)
{
	// Object deserialized reading field by field from the stream

	in >> user.m_username >> user.m_userId >> user.m_nickname
		>> user.m_passwd
		>> user.m_icon
		>> user.m_documents;

	return in;
}

QDataStream& operator<<(QDataStream& out, const User& user)
{
	// Object serialized as the sequence of its member fields

	out << user.m_username << user.m_userId << user.m_nickname
		<< user.m_passwd
		<< user.m_icon
		<< user.m_documents;

	return out;
}

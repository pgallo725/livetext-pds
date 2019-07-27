#include "User.h"

#include <QString>
#include <QDataStream>



User::User(QString username, QString nickname, QString passwd)
	: m_username(username), m_nickname(nickname),
	m_passwd(passwd),
	m_icon(QPixmap())
{
}

User::User(QString username, QString nickname, QString passwd, QPixmap icon)
	: m_username(username), m_nickname(nickname),
	m_passwd(passwd),
	m_icon(icon)
{
}

User::~User()
{
}


QString User::getInfo()
{
	return QString("Username: " + m_username + " - Nickname: " + m_nickname);
}

QString User::getUsername()
{
	return m_username;
}

QString User::getNickName()
{
	return m_nickname;
}

QString User::getPassword()
{
	return m_passwd;
}





void User::addDocument(QString docUri)
{
	m_documents.push_back(docUri);
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

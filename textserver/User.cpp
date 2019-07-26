#include "User.h"

#include <QString>
#include <QDataStream>



User::User(QString username, int userId, QString nickname, QString passwd)
	: m_username(username), m_userId(userId), m_nickname(nickname),
	m_passwd(passwd), m_icon(QPixmap())
{
}

User::User(QString username, int userId, QString nickname, QString passwd, QPixmap icon)
	: m_username(username), m_userId(userId), m_nickname(nickname),
	m_passwd(passwd), m_icon(icon)
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

int User::getUserId()
{
	return m_userId;
}

QString User::getNickName()
{
	return m_nickname;
}

QString User::getPassword()
{
	return m_passwd;
}

QPixmap User::getIcon()
{
	return m_icon;
}


void User::addDocument(QString docUri)
{
	if(!m_documents.contains(docUri))
		m_documents.insert(docUri);
}

void User::setNickname(QString newNickname)
{
	m_nickname = newNickname;
}

void User::deleteNickname()
{
	m_nickname = m_username;
}

void User::setIcon(QPixmap newIcon)
{
	m_icon = newIcon;
}

void User::deleteIcon()
{
	m_icon = QPixmap();
}

void User::changePassword(QString newPassword)
{
	//TODO: decrypt
	m_passwd = newPassword;
}


QDataStream& operator>>(QDataStream& in, User& user)
{
	// Object deserialized reading field by field from the stream

	/*QString username;
	QString nickname;
	QString password;
	QPixmap icon;
	QList<QString> documents;*/

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

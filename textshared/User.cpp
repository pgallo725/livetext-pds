#include "User.h"

#include <QDataStream>
#include <QRandomGenerator>
#include <QCryptographicHash>

// Set of characters that will be used to generate random sequences as nonce
const QString User::saltCharacters = QStringLiteral("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");


User::User() : m_userId(-1)
{
}

User::User(QString username, int userId, QString nickname, QString passwd, QImage icon)
	: m_username(username), m_userId(userId), m_nickname(nickname), m_icon(icon)
{
	for (int i = 0; i < 32; ++i)	// create a 32-character randomly generated salt sequence
	{
		int index = qrand() % saltCharacters.length();
		QChar nextChar = saltCharacters.at(index);
		m_salt.append(nextChar);
	}

	QCryptographicHash hash(QCryptographicHash::Md5);

	hash.addData(passwd.toUtf8());
	hash.addData(m_salt);

	m_passwd = hash.result();
}

User::~User()
{
	// NOTHING
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

QByteArray User::getPasswordHash()
{
	return m_passwd;
}

QImage User::getIcon()
{
	return m_icon;
}

QByteArray User::getSalt()
{
	return m_salt;
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

void User::setIcon(QImage newIcon)
{
	m_icon = newIcon;
}

void User::setPassword(QString newPassword)
{
	m_salt.clear();

	for (int i = 0; i < 32; ++i)	// create a 32-character randomly generated salt sequence
	{
		int index = qrand() % saltCharacters.length();
		QChar nextChar = saltCharacters.at(index);
		m_salt.append(nextChar);
	}

	QCryptographicHash hash(QCryptographicHash::Md5);

	hash.addData(newPassword.toUtf8());
	hash.addData(m_salt);

	m_passwd = hash.result();
}

void User::update(QString nickname, QImage icon, QString password)
{
	if (!nickname.isEmpty())
		setNickname(nickname);
	if (!icon.isNull())
		setIcon(icon);
	if (!password.isEmpty())
		setPassword(password);
}

void User::recoverFrom(User& u)
{
	m_nickname = u.getNickname();
	m_icon = u.getIcon();
	m_passwd = u.getPasswordHash();
	m_salt = u.getSalt();
	m_documents = u.getDocuments();
}



QDataStream& operator>>(QDataStream& in, User& user)
{
	// Object deserialized reading field by field from the stream

	in >> user.m_username >> user.m_userId >> user.m_nickname
		>> user.m_passwd
		>> user.m_salt
		>> user.m_icon
		>> user.m_documents;		

	return in;
}

QDataStream& operator<<(QDataStream& out, const User& user)
{
	// Object serialized as the sequence of its member fields

	out << user.m_username << user.m_userId << user.m_nickname
		<< user.m_passwd
		<< user.m_salt
		<< user.m_icon
		<< user.m_documents;

	return out;
}

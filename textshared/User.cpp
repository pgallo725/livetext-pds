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
	//	int index = qrand() % saltCharacters.length(); 
		int index = QRandomGenerator::global()->generate() % saltCharacters.length();
		QChar nextChar = saltCharacters.at(index);
	//	m_salt.append(nextChar);
		m_salt.append(nextChar.unicode() );
	}

	QCryptographicHash hash(QCryptographicHash::Sha512);

	hash.addData(passwd.toUtf8());
	hash.addData(m_salt);

	m_passwd = hash.result();
}

User::User(QString username, int userId, QString nickname, QByteArray passhash, QByteArray salt, QImage icon)
	: m_username(username), m_userId(userId), m_nickname(nickname), m_passwd(passhash), m_salt(salt), m_icon(icon)
{
}

User::~User()
{
	// NOTHING
}

QString User::getUsername() const
{
	return m_username;
}

int User::getUserId() const
{
	return m_userId;
}

QString User::getNickname() const
{
	return m_nickname;
}

QByteArray User::getPasswordHash() const
{
	return m_passwd;
}

QImage User::getIcon() const
{
	return m_icon;
}

QByteArray User::getSalt() const
{
	return m_salt;
}

bool User::hasDocument(URI uri) const
{
	return m_documents.contains(uri);
}

QList<URI> User::getDocuments() const
{
	return m_documents;
}

URI User::getURIat(int index) const
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
	//	int index = qrand() % saltCharacters.length();
		int index = QRandomGenerator::global()->generate() % saltCharacters.length();
		QChar nextChar = saltCharacters.at(index);
		// m_salt.append(nextChar);
		m_salt.append(nextChar.unicode());
	}

	QCryptographicHash hash(QCryptographicHash::Sha512);

	hash.addData(newPassword.toUtf8());
	hash.addData(m_salt);

	m_passwd = hash.result();
}

void User::update(QString nickname, QImage icon, QString password)
{
	setNickname(nickname);
	if (!icon.isNull())
		setIcon(icon);
	if (!password.isEmpty())
		setPassword(password);
}

void User::rollback(const User& backup)
{
	m_nickname = backup.getNickname();
	m_icon = backup.getIcon();
	m_passwd = backup.getPasswordHash();
	m_salt = backup.getSalt();
	m_documents = backup.getDocuments();
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

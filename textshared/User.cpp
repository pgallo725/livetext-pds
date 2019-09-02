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
	: m_username(username), m_userId(userId), m_nickname(nickname),
	/*m_passwd(passwd),*/ m_icon(icon), m_salt("")
{
	for (int i = 0; i < 32; ++i)	// create a 32-character randomly generated nonce
	{
		int index = qrand() % saltCharacters.length();
		QChar nextChar = saltCharacters.at(index);
		m_salt.append(nextChar);
	}

	QCryptographicHash hash(QCryptographicHash::Md5);

	hash.addData(passwd.toStdString().c_str(), passwd.length());
	hash.addData(m_salt.toStdString().c_str(), m_salt.length());

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

QByteArray User::getPassword()
{
	return m_passwd;
}

QImage User::getIcon()
{
	return m_icon;
}

QString User::getSalt()
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

void User::setPassword(QByteArray newPassword)
{
	if (m_passwd.compare(newPassword)) {
		m_salt = "";

		for (int i = 0; i < 32; ++i)	// create a 32-character randomly generated nonce
		{
			int index = qrand() % saltCharacters.length();
			QChar nextChar = saltCharacters.at(index);
			m_salt.append(nextChar);
		}

		QCryptographicHash hash(QCryptographicHash::Md5);

		hash.addData(newPassword.toStdString().c_str(), newPassword.length());
		hash.addData(m_salt.toStdString().c_str(), m_salt.length());

		m_passwd = hash.result();
	}
}



QDataStream& operator>>(QDataStream& in, User& user)
{
	// Object deserialized reading field by field from the stream

	in >> user.m_username >> user.m_userId >> user.m_nickname
		>> user.m_passwd
		>> user.m_salt
		>> user.m_documents
		>> user.m_icon;		

	return in;
}

QDataStream& operator<<(QDataStream& out, const User& user)
{
	// Object serialized as the sequence of its member fields

	out << user.m_username << user.m_userId << user.m_nickname
		<< user.m_passwd
		<< user.m_salt
		<< user.m_documents
		<< user.m_icon;		

	return out;
}

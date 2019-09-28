#include "AccountMessage.h"


/*************** ACCOUNT CREATE MESSAGE ***************/

AccountCreateMessage::AccountCreateMessage()
	: Message(AccountCreate)
{
}

AccountCreateMessage::AccountCreateMessage(QString username, QString nickname, QImage icon, QString password)
	: Message(AccountCreate), m_username(username), m_nickname(nickname),
	m_icon(icon), m_password(password)
{
}

void AccountCreateMessage::writeContent(QDataStream& stream) const
{
	stream << m_username << m_nickname << m_password << m_icon;
}

void AccountCreateMessage::readFrom(QDataStream& stream)
{
	stream >> m_username >> m_nickname >> m_password >> m_icon;
}

QString AccountCreateMessage::getUsername() const
{
	return m_username;
}

QString AccountCreateMessage::getNickname() const
{
	return m_nickname;
}

QImage AccountCreateMessage::getIcon() const
{
	return m_icon;
}

QString AccountCreateMessage::getPassword() const
{
	return m_password;
}


/*************** ACCOUNT UPDATE MESSAGE ***************/

AccountUpdateMessage::AccountUpdateMessage()
	: Message(AccountUpdate)
{
}

AccountUpdateMessage::AccountUpdateMessage(QString nickname, QImage icon, QString password)
	: Message(AccountUpdate), m_nickname(nickname), m_icon(icon), m_password(password)
{
}

void AccountUpdateMessage::writeContent(QDataStream& stream) const
{
	stream << m_nickname << m_password << m_icon;
}

void AccountUpdateMessage::readFrom(QDataStream& stream)
{
	stream >> m_nickname >> m_password >> m_icon;
}

QString AccountUpdateMessage::getNickname() const
{
	return m_nickname;
}

QImage AccountUpdateMessage::getIcon() const
{
	return m_icon;
}

QString AccountUpdateMessage::getPassword() const
{
	return m_password;
}


/*************** ACCOUNT CONFIRMED MESSAGE ***************/

AccountConfirmedMessage::AccountConfirmedMessage()
	: Message(AccountConfirmed)
{
}

AccountConfirmedMessage::AccountConfirmedMessage(User user)
	: Message(AccountConfirmed), m_user(user)
{
}

void AccountConfirmedMessage::writeContent(QDataStream& stream) const
{
	stream << m_user;
}

void AccountConfirmedMessage::readFrom(QDataStream& stream)
{
	stream >> m_user;
}

User& AccountConfirmedMessage::getUserObj()
{
	return m_user;
}


/*************** ACCOUNT DENIED MESSAGE ***************/

AccountErrorMessage::AccountErrorMessage()
	: Message(AccountError)
{
}

AccountErrorMessage::AccountErrorMessage(QString reason)
	: Message(AccountError), m_error(reason)
{
}

void AccountErrorMessage::writeContent(QDataStream& stream) const
{
	stream << m_error;
}

void AccountErrorMessage::readFrom(QDataStream& stream)
{
	stream >> m_error;
}

QString AccountErrorMessage::getErrorMessage() const
{
	return m_error;
}

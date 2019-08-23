#include "LogoutMessage.h"


/*************** LOGOUT REQUEST MESSAGE ***************/

LogoutRequestMessage::LogoutRequestMessage()
	: Message(LogoutRequest)
{
}

void LogoutRequestMessage::readFrom(QDataStream& stream)
{
	// NO MESSAGE CONTENT
}

void LogoutRequestMessage::sendTo(QTcpSocket* socket) const
{
	QDataStream streamOut(socket);

	streamOut << (quint16)LogoutRequest;
}


/*************** LOGOUT CONFIRMED MESSAGE ***************/

LogoutConfirmedMessage::LogoutConfirmedMessage()
	: Message(LogoutConfirmed)
{
}

void LogoutConfirmedMessage::readFrom(QDataStream& stream)
{
	// NO MESSAGE CONTENT
}

void LogoutConfirmedMessage::sendTo(QTcpSocket* socket) const
{
	QDataStream streamOut(socket);

	streamOut << (quint16)LogoutConfirmed;
}


/*************** LOGOUT DENIED MESSAGE ***************/

LogoutErrorMessage::LogoutErrorMessage()
	: Message(LogoutError)
{
}

LogoutErrorMessage::LogoutErrorMessage(QString reason)
	: Message(LogoutError), m_reason(reason)
{
}

void LogoutErrorMessage::readFrom(QDataStream& stream)
{
	stream >> m_reason;
}

void LogoutErrorMessage::sendTo(QTcpSocket* socket) const
{
	QDataStream streamOut(socket);

	streamOut << (quint16)LogoutError << m_reason;
}

QString LogoutErrorMessage::getErrorMessage() const
{
	return m_reason;
}

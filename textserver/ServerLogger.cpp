#include "ServerLogger.h"

#include <QCoreApplication>
#include <QThread>
#include <QDateTime>


ServerLogger::ServerLogger(TcpServer* owner, LogType type)
	: QDebug((QtMsgType)type)
{
	(this->noquote() << QDate::currentDate().toString(Qt::ISODate)
		<< QTime::currentTime().toString(Qt::ISODateWithMs)).nospace()
		<< " > ";

	if (type == Error)
		this->noquote();	// Errors print exception messages as strings, avoid quoting
	else this->quote();
}

ServerLogger::ServerLogger(WorkSpace* owner, LogType type)
	: QDebug((QtMsgType)type)
{
	(this->noquote() << QDate::currentDate().toString(Qt::ISODate)
		<< QTime::currentTime().toString(Qt::ISODateWithMs)).nospace()
		<< qSetFieldWidth(7) << " >> [TID:" << QThread::currentThreadId() << "] ";

	if (type == Error)
		this->noquote();	// Errors print exception messages as strings, avoid quoting
	else this->quote();
}

ServerLogger::~ServerLogger()
{
	// The message printing (logging) is handled by the QDebug destructor
}

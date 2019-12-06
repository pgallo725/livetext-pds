#include "ServerLogger.h"

#include <QCoreApplication>
#include <QThread>
#include <QDateTime>


ServerLogger::ServerLogger(TcpServer* owner, LogType type)
	: QDebug((QtMsgType)type)
{
	(void) owner;	// suppress "unused parameter" warning

	if (type != Info)
	{
		(this->noquote() << QDate::currentDate().toString(Qt::ISODate)
			<< QTime::currentTime().toString(Qt::ISODateWithMs)).nospace()
			<< " |  Thread: " << qSetFieldWidth(7) << QThread::currentThreadId() 
			<< qSetFieldWidth(0) << "  >  ";
	}
	else this->nospace();

	if (type == Warning)
	{
		*this << "(WARNING) ";
	}

	if (type == Error)
		this->noquote();	// Errors print exception messages as strings, avoid quoting
	else this->quote();
}

ServerLogger::ServerLogger(WorkSpace* owner, LogType type)
	: QDebug((QtMsgType)type)
{
	(void) owner;

	if (type != Info)
	{
		(this->noquote() << QDate::currentDate().toString(Qt::ISODate)
			<< QTime::currentTime().toString(Qt::ISODateWithMs)).nospace()
			<< " |  Thread: " << qSetFieldWidth(7) << QThread::currentThreadId() 
			<< qSetFieldWidth(0) << "  >>  ";
	}
	else this->nospace();

	if (type == Warning)
	{
		*this << "(WARNING) ";
	}

	if (type == Error)
		this->noquote();	// Errors print exception messages as strings, avoid quoting
	else this->quote();
}

ServerLogger::~ServerLogger()
{
	// The message printing (logging) is handled by the QDebug destructor
}


QDebug operator<<(QDebug debug, std::string str)
{
	debug << QString::fromStdString(str);

	return debug;
}

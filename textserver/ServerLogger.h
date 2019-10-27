#pragma once

#include <QDebug>


// Macro for initiating a logging stream of the desired type (default = Debug)
#define Logger(...)		ServerLogger(this, __VA_ARGS__)


// Supported log message types, interchangeable with the corresponding QtMsgType values
enum LogType
{
	Info = 4,
	Debug = 0,
	Warning = 1,
	Error = 2
};


class WorkSpace;
class TcpServer;

class ServerLogger : public QDebug
{
public:

	ServerLogger(TcpServer* owner, LogType type = Debug);
	ServerLogger(WorkSpace* owner, LogType type = Debug);

	~ServerLogger();
};

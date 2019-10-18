#include "ServerException.h"


ServerException::ServerException(std::string msg)
	: std::exception(("(SERVER ERROR) " + msg).c_str())
{
}


StartupException::StartupException(std::string msg)
	: ServerException("textserver was unable to start...\n" + msg)
{
}


DataBaseException::DataBaseException(std::string msg)
	: ServerException(msg)
{
}

DataBaseReadTableException::DataBaseReadTableException(std::string query)
	: DataBaseException("Unable to exec this query: " + query)
{
}

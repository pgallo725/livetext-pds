#include "ServerException.h"


ServerException::ServerException(std::string msg)
	: std::exception(("(SERVER ERROR) " + msg).c_str())
{
}


StartupException::StartupException(std::string msg)
	: ServerException("textserver was unable to start...\n" + msg)
{
}


DataBaseException::DataBaseException(std::string msg, QSqlError err)
	: ServerException(msg)
{
}

QSqlError DataBaseException::getSqlError()
{
	return err;
}

DataBaseReadTableException::DataBaseReadTableException(std::string query, QSqlError err)
	: DataBaseException("Unable to exec this query: " + query, err)
{
}

DataBaseWriteRecordException::DataBaseWriteRecordException(std::string query, QSqlError err)
	: DataBaseException("Unable to exec this query: " + query, err)
{
}

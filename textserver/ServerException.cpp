#include "ServerException.h"


ServerException::ServerException(std::string msg)
	: std::exception(msg.c_str())
{
}


StartupException::StartupException(std::string msg)
	: ServerException("textserver was unable to start...\n(ERROR) " + msg)
{
}


DatabaseException::DatabaseException(std::string msg, QSqlError err)
	: ServerException("(DB ERROR) " + msg)
{
}

QSqlError& DatabaseException::getSqlError()
{
	return err;
}

DatabaseConnectionException::DatabaseConnectionException(QSqlError err)
	: DatabaseException("Cannot establish database connection", err)
{
}

DatabaseCreateException::DatabaseCreateException(std::string query, QSqlError err)
	: DatabaseException("Creation query failed: " + query, err)
{
}

DatabaseReadException::DatabaseReadException(std::string query, QSqlError err)
	: DatabaseException("Unable to exec this query: " + query, err)
{
}

DatabaseWriteException::DatabaseWriteException(std::string query, QSqlError err)
	: DatabaseException("Unable to exec this query: " + query, err)
{
}

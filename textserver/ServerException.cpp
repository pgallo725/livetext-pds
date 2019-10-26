#include "ServerException.h"


ServerException::ServerException(std::string msg)
	: std::exception(("(SERVER ERROR) " + msg).c_str())
{
}


StartupException::StartupException(std::string msg)
	: ServerException("textserver was unable to start...\n" + msg)
{
}


DatabaseException::DatabaseException(std::string msg, QSqlError err)
	: ServerException(msg)
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

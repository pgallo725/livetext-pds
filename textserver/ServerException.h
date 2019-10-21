#pragma once

#include <exception>
#include <string>
#include <QSqlError>


class ServerException : public std::exception
{

protected:

	ServerException(std::string msg);

};


class StartupException : public ServerException
{

public:

	StartupException(std::string msg);
};


class DataBaseException : public ServerException
{
private:
	QSqlError err;
protected:
	DataBaseException(std::string msg, QSqlError err);
public:
	QSqlError getSqlError();
};

class DataBaseReadTableException : public DataBaseException
{
public:
	DataBaseReadTableException(std::string query, QSqlError err);
};

class DataBaseWriteRecordException : public DataBaseException
{
public:
	DataBaseWriteRecordException(std::string query, QSqlError err);
};
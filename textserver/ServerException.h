#pragma once

#include <exception>
#include <string>


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
protected:
	DataBaseException(std::string msg);
};

class DataBaseReadTableException : public DataBaseException
{
public:
	DataBaseReadTableException(std::string query);
};
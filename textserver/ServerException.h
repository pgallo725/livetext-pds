#pragma once

#ifndef __SERVER_EXCEPTION__
#define __SERVER_EXCEPTION__

#include <exception>
#include <string>
#include <QSqlError>


/******************************************************************

	Exception hierarchy tree diagram:

						   ServerException
							/			\
				StartupException		DatabaseException
										/	 /	  \		\
									Create  Open  Read	Write


*******************************************************************/


class ServerException : public std::exception
{
private :
    std::string m_message;
   public:
    virtual const char* what() const noexcept;
protected:

	ServerException(std::string msg);
};


class StartupException : public ServerException
{
public:

	StartupException(std::string msg);
};


class DatabaseException : public ServerException
{
private:

	QSqlError err;

protected:

	DatabaseException(std::string msg, QSqlError err);

public:

	QSqlError& getSqlError();
};

class DatabaseConnectionException : public DatabaseException
{
public:

	DatabaseConnectionException(QSqlError err);
};

class DatabaseCreateException : public DatabaseException
{
public:

	DatabaseCreateException(std::string query, QSqlError err);
};

class DatabaseReadException : public DatabaseException
{
public:

	DatabaseReadException(std::string query, QSqlError err);
};

class DatabaseWriteException : public DatabaseException
{
public:

	DatabaseWriteException(std::string query, QSqlError err);
};
#endif

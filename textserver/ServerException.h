#pragma once

#include <exception>
#include <string>


class ServerException : public std::exception
{

protected:

	ServerException(std::string msg);
};


class FileException : public ServerException
{

protected:

	FileException(std::string msg);
};


class FileLoadException : public ServerException
{

public:

	FileLoadException(std::string filePath);
};


class FileWriteException : public ServerException
{

public:

	FileWriteException(std::string fileName, std::string path);
};


class FileOverwriteException : public ServerException
{

public:

	FileOverwriteException(std::string filePath);
};


class MessageException : public ServerException
{
public:
	MessageException(std::string msg);
};


class MessageUnknownTypeException : public MessageException
{
public:
	MessageUnknownTypeException(int type);
};

class MessageWrongTypeException : public MessageException
{
public:
	MessageWrongTypeException(std::string msg, int type);
};

class SocketNullException : public ServerException
{
public:
	SocketNullException(std::string msg);
};
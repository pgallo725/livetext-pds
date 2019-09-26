#pragma once

#include <exception>
#include <string>


class ServerException : public std::exception
{

protected:

	ServerException(std::string msg);

};


class ServerStartException : public ServerException
{
public:
	ServerStartException(std::string msg);
};

class FileException : public ServerException
{

protected:

	FileException(std::string msg);
};


class FileLoadException : public FileException
{

public:

	FileLoadException(std::string filePath);
};


class FileWriteException : public FileException
{
private:
	std::string path;
public:
	FileWriteException(std::string fileName, std::string path);
	std::string getPath();
};


class FileCreateException : public FileException
{
public:
	FileCreateException(std::string fileName, std::string path);
};


class FileOpenException : public FileException
{
public:
	FileOpenException(std::string fileName, std::string path);
};


class FileOverwriteException : public FileException
{
public:
	FileOverwriteException(std::string filePath);
};


class MessageException : public ServerException
{
public:
	MessageException(std::string msg);
};


class MessageUnexpectedTypeException : public MessageException
{
private:

	int errType;

public:
	MessageUnexpectedTypeException(int type);
	int getErrType();
};




class SocketException : public ServerException
{
protected:
	SocketException(std::string msg);
};


class SocketNullException : public SocketException
{
public:
	SocketNullException(std::string msg);
};


class SocketDuplicateException : public SocketException
{
public:
	SocketDuplicateException(std::string msg);
};


class UserNotFoundException : public ServerException
{
public:
	UserNotFoundException(std::string msg);
};
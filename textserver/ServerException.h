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


class SocketException : public ServerException
{

public:

	SocketException(std::string msg);
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

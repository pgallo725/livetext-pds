#pragma once

#include <exception>
#include <string>

class DocumentException : public std::exception
{
protected:
	DocumentException(std::string msg);
};

class DocumentOpenException : DocumentException
{
public:
	DocumentOpenException(std::string fileName, std::string path);
};

class DocumentLoadException : DocumentException
{
public:
	DocumentLoadException(std::string fileName, std::string path);
};

class DocumentWriteException : DocumentException
{
public:
	DocumentWriteException(std::string fileName, std::string path);
};
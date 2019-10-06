#pragma once

#include <exception>
#include <string>
#include "Message.h"

class DocumentException : public std::exception
{
protected:
	DocumentException(std::string msg);
};

class DocumentOpenException : public DocumentException
{
public:
	DocumentOpenException(std::string fileName, std::string path);
};

class DocumentCreateException : public DocumentException
{
public:
	DocumentCreateException(std::string fileName, std::string path);
};

class DocumentLoadException : public DocumentException
{
public:
	DocumentLoadException(std::string fileName, std::string path);
};

class DocumentWriteException : public DocumentException
{
public:
	DocumentWriteException(std::string fileName, std::string path);
};

class MessageException : public std::exception
{
protected:
	MessageException(std::string msg);
};

class MessageReadException : public MessageException
{
public:
	MessageReadException(std::string msg, MessageType m_type);
};
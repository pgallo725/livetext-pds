#pragma once

#include <exception>
#include <string>
#include "Message.h"


/******************************************************************

	Exception hierarchy tree diagram:

						   SharedException
							/			\
			DocumentException			MessageException
			/   /      \	\			/		\		\
	   Create  Open   Load  Write	  Type		Read	Write


*******************************************************************/


class SharedException : public std::exception
{
protected:

	SharedException(std::string msg);
};


class DocumentException : public SharedException
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


class MessageException : public SharedException
{
protected:

	MessageException(std::string msg);
};

class MessageReadException : public MessageException
{
public:

	MessageReadException(std::string msg, MessageType type);
};

class MessageWriteException : public MessageException
{
public:

	MessageWriteException(std::string msg, MessageType type);
};

class MessageTypeException : public MessageException
{
public:

	MessageTypeException(MessageType type);
};
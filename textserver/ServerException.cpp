#include "ServerException.h"


ServerException::ServerException(std::string msg)
	: std::exception(("SERVER ERROR: " + msg).c_str())
{
}


FileException::FileException(std::string msg)
	: ServerException(msg)
{
}

FileLoadException::FileLoadException(std::string filePath)
	: ServerException("Unable to read file " + filePath)
{
}

FileWriteException::FileWriteException(std::string fileName, std::string path)
	: ServerException("Unable to create or write to file '" + fileName + "' in " + path)
{
}

FileOverwriteException::FileOverwriteException(std::string filePath)
	: ServerException("Unable to overwrite file " + filePath)
{
}

MessageException::MessageException(std::string msg) : 
	ServerException(msg)
{
}

MessageUnknownTypeException::MessageUnknownTypeException(int type) :
	MessageException("Unknown message type : " + type)
{
}

MessageWrongTypeException::MessageWrongTypeException(std::string msg, int type) :
	MessageException(msg + std::to_string(type))
{
}

SocketNullException::SocketNullException(std::string msg) :
	ServerException(msg)
{
}



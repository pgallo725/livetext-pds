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
	: ServerException("Unable to write file '" + fileName + "' in " + path)
{
}

FileCreateException::FileCreateException(std::string fileName, std::string path)
	: FileException("Unable to create file '" + fileName + "' in " + path)
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
	MessageException("Unknown message type : " + type), errType(type)
{
}

int MessageUnknownTypeException::getErrType()
{
	return errType;
}

MessageWrongTypeException::MessageWrongTypeException(std::string msg, int type) :
	MessageException(msg + std::to_string(type))
{
}

FieldWrongException::FieldWrongException(int type) :
	MessageException("Wrong field type : " + type), type(type)
{
}

SocketNullException::SocketNullException(std::string msg) :
	ServerException(msg)
{
}



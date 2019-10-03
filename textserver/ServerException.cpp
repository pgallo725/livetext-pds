#include "ServerException.h"


ServerException::ServerException(std::string msg)
	: std::exception(("SERVER ERROR: " + msg).c_str())
{
}


StartupException::StartupException(std::string msg)
	: ServerException("textserver was unable to start...\n" + msg)
{
}


SocketException::SocketException(std::string msg) :
	ServerException(msg)
{
}


FileException::FileException(std::string msg)
	: ServerException(msg)
{
}

FileLoadException::FileLoadException(std::string filePath)
	: FileException("Unable to read file " + filePath)
{
}

FileWriteException::FileWriteException(std::string fileName, std::string path)
	: FileException("Unable to write file '" + fileName + "' in " + path), path(path)
{
}

std::string FileWriteException::getPath()
{
	return path;
}

FileCreateException::FileCreateException(std::string fileName, std::string path)
	: FileException("Unable to create file '" + fileName + "' in " + path)
{
}

FileOpenException::FileOpenException(std::string fileName, std::string path)
	: FileException("Unable to open file '" + fileName + "' in " + path)
{
}

FileOverwriteException::FileOverwriteException(std::string filePath)
	: FileException("Unable to overwrite file " + filePath)
{
}

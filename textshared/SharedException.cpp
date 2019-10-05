#include "SharedException.h"

DocumentException::DocumentException(std::string msg)
	: std::exception(("(DOCUMENT ERROR) " + msg).c_str())
{
}

DocumentOpenException::DocumentOpenException(std::string fileName, std::string path)
	: DocumentException("Unable to open file '" + fileName + "' in " + path)
{
}

DocumentCreateException::DocumentCreateException(std::string fileName, std::string path)
	: DocumentException("Unable to create or overwrite file '" + fileName + "' in " + path)
{
}

DocumentLoadException::DocumentLoadException(std::string fileName, std::string path)
	: DocumentException("Unable to load file '" + fileName + "' in " + path)
{
}

DocumentWriteException::DocumentWriteException(std::string fileName, std::string path)
	: DocumentException("Unable to write file '" + fileName + "' in " + path)
{
}

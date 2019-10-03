#include "SharedException.h"

DocumentException::DocumentException(std::string msg)
	: std::exception(("DOCUEMENT ERROR: " + msg).c_str())
{
}

DocumentOpenException::DocumentOpenException(std::string fileName, std::string path)
	: DocumentException("Unable to open file '" + fileName + "' in " + path)
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

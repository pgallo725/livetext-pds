#include "SharedException.h"


SharedException::SharedException(std::string msg)
    :exception()
{
    m_message=msg;
}

 const char* SharedException::what()const noexcept
{
    return m_message.c_str();
}

DocumentException::DocumentException(std::string msg)
	: SharedException("(DOCUMENT ERROR) " + msg)
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
	: DocumentException("Unable to load file '" + fileName + "' from " + path)
{
}

DocumentWriteException::DocumentWriteException(std::string fileName, std::string path)
	: DocumentException("Unable to write file '" + fileName + "' in " + path)
{
}


MessageException::MessageException(std::string msg)
	: SharedException("(MESSAGE ERROR) " + msg)
{
}

MessageReadException::MessageReadException(std::string msg, MessageType m_type)
	: MessageException(msg + " (" + Message::TypeName(m_type) + ")")
{
}

MessageWriteException::MessageWriteException(std::string msg, MessageType m_type)
	: MessageException(msg + " (" + Message::TypeName(m_type) + ")")
{
}

MessageTypeException::MessageTypeException(MessageType m_type)
	: MessageException("Unknown message type : " + std::to_string(m_type))
{
}

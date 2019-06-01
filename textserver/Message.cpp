#include "Message.h"

Message::Message(std::string message)
{
	msg = message;
}

Message::~Message()
{
}

int Message::getType()
{
	return 0;
}

int Message::getIP()
{
	return 0;
}

int Message::getPort()
{
	return 0;
}

std::string Message::getUserName()
{
	return msg;
}

std::string Message::getName()
{
	return std::string();
}

std::string Message::getSurname()
{
	return std::string();
}

std::string Message::getPasswd()
{
	return std::string();
}

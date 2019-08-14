#pragma once

#include <QTcpSocket>

#include <memory>
#include "Message.h"


// TEMPORARY forward-declarations
class WorkSpace;
class TcpServer;

class MessageHandler
{

public:

	enum OwnerType
	{
		Server,
		Workspace
	};

private:

	WorkSpace* workspace;	// TEMPORARY
	OwnerType _scenario;

public:

	MessageHandler(OwnerType t, WorkSpace* w);

	void process(std::unique_ptr<Message>&& msg, QTcpSocket* sender);

	~MessageHandler();
};
#pragma once

#include <list>
#include <queue>
#include <mutex>
#include "Document.h"
#include "Client.h"
#include "Message.h"

class WorkSpace
{
private:
	Document* doc;
	std::list<Client*> editors;
	std::queue<Message> msgQueue;

	std::mutex mQueue;

public:
	WorkSpace(Document* d, Client* author);
	~WorkSpace();

	void addMessageToQueue(Message msg);	/* threads safe */
};


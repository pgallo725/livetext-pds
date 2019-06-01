#include "WorkSpace.h"

WorkSpace::WorkSpace(Document* d, Client* author): doc(d)
{
	editors.push_back(author);	/* first element in the list is the author */
}

WorkSpace::~WorkSpace()
{
}

void WorkSpace::addMessageToQueue(Message msg)
{
	std::unique_lock<std::mutex> ul(mQueue);
	msgQueue.push(msg);
}

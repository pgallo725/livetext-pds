#include "Client.h"

Client::Client(int id, QTcpSocket* s, User* u):
	clientId(id), socket(s), activeUser(u)
{
}

Client::~Client()
{
}

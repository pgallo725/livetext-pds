#include "User.h"

User::User(std::string name, std::string surname, std::string username, 
	std::string passwd, int siteId, int color):
	name(name), surname(surname), username(username),
	passwd(passwd), _siteId(siteId), color(color), _counter(0)
{
	// TODO: add image init
}

User::~User()
{
}

std::string User::getUserInfo()
{
	return std::string("name:"+name+" - surname:"+surname+" - nickname:"+username);
}

bool User::authentication(std::string passwd)
{
	// TODO check and pars on passwd

	if (!this->passwd.compare(passwd)) {
		return true;
	}
	return false;
}

int User::getSiteId()
{
	return _siteId;
}

void User::addDocument(std::string doc)
{
	documents.push_back(doc);
}

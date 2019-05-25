#include "User.h"

User::User(std::string name, std::string surname, std::string username, std::string passwd, int siteId):
	name(name), surname(surname), username(username), passwd(passwd), _siteId(siteId), _counter(0)
{
	// TODO: add image
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

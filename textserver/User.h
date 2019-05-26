#pragma once

#include <string>

class User
{
private:
	std::string name;
	std::string surname;
	std::string username;
	std::string passwd;
	int _siteId;
	int _counter;
public:
	User(std::string name, std::string surname, std::string username, std::string passwd, int siteId);
	~User();

	std::string getUserInfo();
	bool authentication(std::string passwd);
	int getSiteId();
};


#pragma once

#include <qobject.h>
#include <string>
#include <list>

class User
{
private:
	std::string name;
	std::string surname;
	std::string username;
	std::string passwd;
	int _siteId;
	int _counter;
	int color;
	QPixmap *icon;
	std::list<std::string> documents;
public:
	User(std::string name, std::string surname, std::string username,
		std::string passwd, int siteId, int color);
	~User();

	/* getter methods */
	std::string getUserInfo();
	int getSiteId();

	/* setter methods */
	void addDocument(std::string doc);

	/* geeneral methods */
	bool authentication(std::string passwd);
	
};


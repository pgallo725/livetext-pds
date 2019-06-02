#pragma once

#include <string>

class Document
{
private:
	std::string docName;
	int docId;
	// TODO: document structur

public:
	Document(std::string name, int id);
	~Document();
};


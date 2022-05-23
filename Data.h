#pragma once

#include <fstream>
#include <string>
#include <list>

class Data
{
	Data() {}
	Data(const Data&);
	Data& operator=(Data&);

	std::list <std::string> exceptions;
	std::string repository;

	long long fileAgeLimits = 0;

public:

	static Data& getInstance();

	void loadData(std::string fileName);

	void saveData(std::string fileName);

	std::list<std::string> getExceptions();

	void addException(std::string fileName);

	void deleteException(std::string fileName);

	std::string getRepository();

	void setRepository(std::string fileName);

	void setFileAgeLimits(long long minutes);

	long long getFileAgeLimits();


};


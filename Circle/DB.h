#pragma once
#include <Windows.h>  

class DB {
public:
	static DB& getDB();

	static unsigned int getNewId();

private:
	DB();
	~DB();

private:
	static DB* sigleton;
	static unsigned int lastId;
};
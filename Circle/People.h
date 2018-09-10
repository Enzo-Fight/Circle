#pragma once
#include<string>
#include"Msg.h"
using std::string;

class People {
public:
	People(string name,string password);
	~People();

	const string& getName();
	const unsigned int getId();

private:
	const unsigned int id;
	string name;
	string password;
};

#pragma once
#include<string>
#include"Msg.h"
using std::string;

class People {
public:
	People(string name,string password);
	~People();

	const string getName() { return name; }
	constexpr unsigned int getId() {
		return id;
	}

private:
	const unsigned int id;
	string name;
	string password;

};

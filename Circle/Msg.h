#pragma once
#include<string>
#include<iostream>
using std::string;
using std::cout;
using std::endl;
using std::to_string;

class Msg {
public:
	Msg(string content) :content(content) {}
	~Msg() {}
	const string& getContent() { return content; }
private:
	string content;
};

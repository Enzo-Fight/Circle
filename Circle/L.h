#pragma once
#include<iostream>
#include<string>
using std::string;

class L {
public:
	static const unsigned int VERBOSE = 1;
	static const unsigned int DEBUG = 2;
	static const unsigned int INFO = 3;
	static const unsigned int WARN = 4;
	static const unsigned int ERROR = 5;
	static const unsigned int NOTHING = 6;
	static unsigned int level;

	static void v(string tag, string msg);
	static void d(string tag, string msg);
	static void i(string tag, string msg);
	static void w(string tag, string msg);
	static void e(string tag, string msg);
};

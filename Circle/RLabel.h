#pragma once
#include<string>
#include<set>
using std::string;
using std::set;
//rating label 等级标签
class RLabel {
	friend class ChatPool;
private:
	RLabel(const string& labelName, int labelGrade);
	RLabel(const string& labelName, int labelGrade, set<unsigned int>* clientGroup);
	~RLabel();
	void addClient(unsigned int id);
	void delClient(unsigned int id);

public:
	int getGrade();

private:
	string name;				//标签名
	int grade;					//等级
	set<unsigned int>*clients;   //标签包含哪些人
};
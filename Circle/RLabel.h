#pragma once
#include<string>
#include<set>
using std::string;
using std::set;
//rating label 等级标签

class Client;

class RLabel {
	friend class ChatPool;
private:
	RLabel(const string& labelName, int labelGrade);
	RLabel(const string& labelName, int labelGrade, set<Client*>* clientGroup);
	~RLabel();
	void addClient(Client* client);
	void delClient(Client* client);

public:
	int getGrade();

private:
	string name;				//标签名
	int grade;					//等级
	set<Client*>*clients;		 //标签包含哪些人
};
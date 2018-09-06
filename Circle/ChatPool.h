#pragma once
#include<string>
#include<queue>
#include<vector>
#include"File.h"
using std::string;
using std::queue;
using std::vector;

class Msg;
class Client;

//rating label 等级标签
class RLabel {
public:
	int getGrade() { return grade; }
	friend class ChatPool;
private:
	RLabel(const string& labelName,int labelGrade) : name(labelName), grade(labelGrade){}
	~RLabel() {}

private:
	string name;				//标签名
	int grade;					//等级
	vector<unsigned int>*group;	//标签包含哪些人
};

//聊天池
class ChatPool {
public:
	ChatPool(const string& poolName);
	~ChatPool();

	RLabel* createLabel(unsigned int clientId, int myGrade, const string& labelName,
		int labelGrade, vector<Client&>&labelGroup);

	string name;
	queue<Msg*>msgs;

private:
	const unsigned int poolId;
	vector<RLabel*>labels;
};

//不同用户在同一聊天池中的等级不一样，属性不能放在聊天池中
class ChatPoolAttr {
public:
	ChatPoolAttr() {}
	~ChatPoolAttr() {}

	RLabel *label;				//所属等级标签
	char time[8];				//缓存最近访问聊天池的时间
};

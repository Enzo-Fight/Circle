#pragma once
#include"People.h"
#include"ChatPool.h"
#include<queue>
#include<map>
#include<list>
#include<set>
using std::queue;
using std::map;
using std::list;
using std::vector;
using std::set;
using std::to_string;

#define MAX_LABEL_NAME_LENGTH 12

class PoolAttr;
class RLabel;

class Client : public People{
public:
	Client(string name, string password,unsigned long long phoneNum);
	~Client();

	bool chat(Client& other,Msg& msg);
	bool noticeMsg(Client& fromUser,Msg& msg);

	//主动申请成为朋友
	void addFriend(Client& fri);
	//请求成为朋友
	void reqAddFri(Client& fri);
	//通知申请朋友是否成功
	void noticeAddFri(Client& fri, bool isSuccess);
	bool delFriend(Client& fri);

	void print();

private:
	unsigned long long phoneNum;
	map<unsigned int,queue<const Msg*,list<const Msg*>>*>frisMsgQ;	//朋友及聊天信息
	static unsigned int maxSizeOfMsgQ;								//私聊聊天信息最大容量

	bool _dealMsg(const unsigned int id, Msg* msg,bool isRecv);

public:
	//群聊
	RLabel * createLabel(const string& labelName);
	bool setRatingLabel(ChatPool* pool, vector<Client*>*clientG, RLabel* label);
	bool addFather(Client* father, ChatPool* chatPool);
	bool addChild(Client* child, ChatPool* chatPool);
	bool sendPoolMsg(ChatPool* chatPool, Msg* msg);
	vector<Msg*>* getPoolMsg(ChatPool* chatPool);

private:
	//群聊属性
	set<PoolAttr*> chatPools;	//所在的所有聊天池
	vector<Client*>fathers;			//所有的父节点
	vector<Client*>children;			//所有的子节点

	bool _addFather(Client* father, ChatPool* chatPool);
	bool _addChild(Client* child, ChatPool* chatPool);
	bool labelIsLegal(Client* child, Client* father, ChatPool* chatPool);
};

struct PoolAttr {
	ChatPool *poolAddr;			//所在聊天池
	RLabel *label;				//所属等级标签
	char time[8];				//缓存最近访问聊天池的时间
};

//rating label 等级标签
class RLabel {
public:
	RLabel(const string& labelName) : name(labelName) {}
	~RLabel() {}
	int getGrade() { return grade; }

private:
	string name;			//标签名
	int grade;				//等级
	vector<Client*>group;	//标签包含哪些人  Client*
};
#pragma once
#include"People.h"
#include"File.h"
#include<queue>
#include<map>
#include<list>
#include<set>
#include"ChatPool.h"

using std::queue;
using std::map;
using std::list;
using std::vector;
using std::set;
using std::to_string;

#define MAX_LABEL_NAME_LENGTH 12

class Client : public People{

public:
	Client(string name, string password,unsigned long long phoneNum);
	~Client();

/* 私聊 */
public:
	bool chat(Client& other,Msg& msg);
	
	//添加好友
	void addFriend(Client& fri);

	//请求添加好友
	void reqAddFri(Client& fri);

	//回调，通知添加好友是否成功
	void noticeAddFri(Client& fri, bool isSuccess);

	//删除好友
	bool delFriend(unsigned int friId);

	//测试——打印所有朋友的消息队列大小
	void print();

private:
	unsigned long long phoneNum;									//手机号码
	map<unsigned int,queue<const Msg*,list<const Msg*>>*>frisMsgQ;	//朋友及聊天信息
	static unsigned int maxSizeOfMsgQ;								//私聊聊天信息最大容量

	bool _dealMsg(const unsigned int id, Msg* msg,bool isRecv);		//处理私聊消息

/* 金字塔通信 */
public:
	//创建聊天池
	ChatPool& createChatPool(const string& name);

	//创建等级标签
	RLabel* createLabel(ChatPool* chatPool, const string& labelName);

	//设置聊天池中的等级标签
	bool setRatingLabel(ChatPool* pool, vector<Client*>*clientG, RLabel* label);

	//添加一个父节点
	bool addFather(Client* father, ChatPool* chatPool);

	//添加一个子节点
	bool addChild(Client* child, ChatPool* chatPool);

	//在聊天池中发一条消息
	bool sendPoolMsg(ChatPool* chatPool, Msg* msg);

	//获取聊天池消息
	vector<Msg*>* getPoolMsg(ChatPool* chatPool);

private:
	map<ChatPool*, ChatPoolAttr*> chatPools;	//所在的所有聊天池
	vector<ChatPool*>mChatPools;				//所掌管的聊天池(合成金字塔时需考虑聊天池拥有者的问题)
	vector<Client*>fathers;						//所有的父节点
	vector<Client*>children;					//所有的子节点

	//相互添加节点
	bool _addFather(Client* father, ChatPool* chatPool);
	bool _addChild(Client* child, ChatPool* chatPool);

	//判断两个节点是否可以构成父子关系（根据等级标签）
	bool labelIsLegal(Client* child, Client* father, ChatPool* chatPool);
};

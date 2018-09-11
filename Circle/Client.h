#pragma once
#include"People.h"
#include<queue>
#include<map>
#include<list>
#include<set>

using std::queue;
using std::map;
using std::list;
using std::vector;
using std::set;

class ChatPool;
class ChatPoolAttr;
class RLabel;
class PoolMsg;

class Client : public People{
public:
	Client(string name, string password, unsigned long long
		phoneNum) :People(name, password), phoneNum(phoneNum) {}
	~Client() {}

/* 私聊 */
public:
	virtual bool chat(Client* other, Msg& msg) = 0;
	virtual bool _dealMsg(const unsigned int id, Msg* msg, bool isRecv) = 0;		//处理私聊消息

	//请求添加好友
	virtual void reqAddFri(Client* fri) = 0;
	virtual void _addFriend(Client* fri) = 0;

	//回调，通知添加好友是否成功
	virtual void noticeAddFri(Client* fri, bool isSuccess) = 0;

	//删除好友
	virtual bool delFriend(unsigned int friId) = 0;

protected:
	unsigned long long phoneNum;									//手机号码
	map<unsigned int,queue<const Msg*,list<const Msg*>>*>frisMsgQ;	//朋友及聊天信息
	static const unsigned int maxSizeOfMsgQ = 100;					//私聊聊天信息最大容量

/* 金字塔通信 */
public:
	//创建聊天池
	virtual ChatPool* createChatPool(const string& name) = 0;

	//邀请加入聊天池
	virtual void inviteJoinPool(Client* mFriend, ChatPool* chatPool,RLabel* label) = 0;

	//退出聊天池
	virtual void leavePool(ChatPool* chatPool) = 0;

	//添加用户关系
	virtual void addFather(Client * father, ChatPool* chatPool) = 0;
	virtual void addChild(Client * child,ChatPool* chatPool) = 0;

	//创建等级标签
	virtual RLabel* createLabel(ChatPool * chatPool, const string & labelName, const int labelGrade) = 0;

	//设置聊天池中的等级标签
	virtual Client* setRatingLabel(ChatPool* chatPool, RLabel* label, set<Client*>*clientG) = 0;

	//在聊天池中发一条消息
	virtual void sendPoolMsg(ChatPool* chatPool, Msg* msg, RLabel * label) = 0;

	//获取聊天池消息
	virtual vector<PoolMsg*>* getPoolMsg(ChatPool* chatPool) = 0;

	//询问父节点消息发送者id是否是自己的祖辈，递归调用
	virtual bool askParent(ChatPool*chatPool,const unsigned int id,const int grade) = 0;
	
	//加入聊天池
	virtual void _joinPool(Client* somebody, ChatPool* chatPool, RLabel* label) = 0;

	map<ChatPool*, ChatPoolAttr*> chatPools;	//所在的所有聊天池
	set<ChatPool*>mChatPools;					//所掌管的聊天池(合成金字塔时需考虑聊天池拥有者的问题)
};

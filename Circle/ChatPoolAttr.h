#pragma once
#include <windows.h>
#include"Client.h"
#include"RLabel.h"
//不同用户在同一聊天池中的等级不一样，属性不能放在聊天池中
class ChatPoolAttr {
	friend class ChatClient;
	friend class Client;
public:
	//cacheTime 的初始化时间待更改，改为系统当前时间
	ChatPoolAttr(RLabel *label); 
	~ChatPoolAttr();
	int getGrade();

	//添加一个父节点
	void addFather(Client* father);

	//添加一个子节点
	void addChild(Client* child);

	const unsigned long getReqTime();
private:
	void setCacheTime(const unsigned int time);
	vector<Client*>fathers;			//所有的父节点
	vector<Client*>children;		//所有的子节点
	RLabel *mLabel;					//所属等级标签
	SYSTEMTIME sysTime;
	unsigned long cacheTime;				//缓存最近访问聊天池的时间
};

#pragma once
#include<string>
#include<queue>
#include<vector>
#include<map>
#include"File.h"
#include"Client.h"
#include"RLabel.h"
#include"PoolMsg.h"
#include"ChatPoolAttr.h"
#include <windows.h>
using std::string;
using std::queue;
using std::vector;
using std::map;

//聊天池
class ChatPool {
	friend class Client;
	friend class ChatClient;
public:
	const static unsigned int tSec = 1000;
	const static unsigned int tMin = 60 * tSec;
	const static unsigned int tHour = 60 * tMin;
	const static unsigned int tDay = 24 * tHour;
	const static unsigned int tMon = 31 * tDay;

private:
	ChatPool(const string & poolName, Client* sponsor);
	~ChatPool();

	const unsigned int getId();
	const string& getName();

	void addClient(Client* client,RLabel* label);
	void delClient(Client* client, RLabel* label);
	void noticeJoin(Client* sponsor, Client* invitedMan, RLabel* label);
	
	RLabel* createLabel(const int myGrade, const string& labelName,
		int labelGrade, set<Client*>*labelGroup);
	RLabel* createLabel(const int myGrade, const string& labelName,
		int labelGrade);
	bool setRatingLabel(const int myGrade, RLabel* label, 
		set<Client*>* labelGroup);

	void addMsg(PoolMsg * msg);
	vector<PoolMsg*>* getMsg(Client* client);
	bool hasClients(set<Client*>* clients);
	
private:
	static unsigned int maxSizeOfMsgQ;
	const unsigned int poolId;
	string name;
	set<Client*> mClients;		//包括用户id和用户名称
	set<RLabel*>labels;
	map<unsigned long,PoolMsg*>msgs;		//消息按时间排序un
	SYSTEMTIME sysTime;
};
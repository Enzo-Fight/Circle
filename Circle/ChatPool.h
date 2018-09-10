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
private:
	ChatPool(const string & poolName, unsigned int sponsorId,
		const string& sponsorName);
	~ChatPool();

	const unsigned int getId();
	const string& getName();

	void addClient(unsigned int clientId, const string& clientName, 
		RLabel* label);
	void delClient(unsigned int clientId, RLabel* label);
	void noticeJoin(unsigned int sponsorId, const string &
		sponsorName, unsigned int invitedManId, 
		const string & invitedManName,RLabel* label);
	
	RLabel* createLabel(const int myGrade, const string& labelName,
		int labelGrade, set<unsigned int>*labelGroup);
	RLabel* createLabel(const int myGrade, const string& labelName,
		int labelGrade);
	bool setRatingLabel(const int myGrade, RLabel* label, 
		set<unsigned int>* labelGroup);

	void addMsg(PoolMsg * msg);
	vector<PoolMsg*>* getMsg(Client* client);
private:
	static unsigned int maxSizeOfMsgQ;

	const unsigned int poolId;
	string name;
	map<unsigned int,string> mClients;
	set<RLabel*>labels;
	map<unsigned long long,PoolMsg*>msgs;		//消息按时间排序un
	SYSTEMTIME sysTime;
	const static unsigned int tSec = 1000;
	const static unsigned int tMin = 60 * tSec;
	const static unsigned int tHour = 60 * tMin;
	const static unsigned int tDay = 24 * tHour;
	const static unsigned int tMon = 31 * tDay;
	bool hasClients(set<unsigned int>* clients);
};
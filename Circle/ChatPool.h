#pragma once
#include<string>
#include<queue>
#include<vector>
#include<map>
#include"File.h"
#include"Client.h"
#include"RLabel.h"
using std::string;
using std::queue;
using std::vector;
using std::map;

class Msg;

//ÁÄÌì³Ø
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

private:
	const unsigned int poolId;
	string name;
	map<unsigned int,string> mClients;
	set<RLabel*>labels;
	queue<Msg*>msgs;

	bool hasClients(set<unsigned int>* clients);

};
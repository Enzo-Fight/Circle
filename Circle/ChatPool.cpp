#include "ChatPool.h"
#include<iostream>
using std::cout;
using std::endl;
using std::cerr;

ChatPool::ChatPool(const string & poolName,Client* sponsor) : name(poolName),
	poolId(File::getNewChatPoolId()) {
	mClients.insert(sponsor);
}

ChatPool::~ChatPool() {
	//清除所有的标签
	for (auto it = labels.begin(); it != labels.end(); ++it) {
		delete *it;
	}labels.clear();
	//清除所有的聊天记录
	while (msgs.size()) {
		auto it = --msgs.end();
		delete it->second;
		msgs.erase(it);
	}
}

const unsigned int ChatPool::getId() {
	return poolId;
}

const string & ChatPool::getName() {
	return name;
}

void ChatPool::addClient(Client* client, RLabel* label) {
	mClients.insert(client);
	label->addClient(client);
}

void ChatPool::delClient(Client* client, RLabel* label) {
	mClients.erase(client);
	label->delClient(client);
}

void ChatPool::noticeJoin(Client* sponsor,Client* invitedMan, RLabel* label) {
	cout << getId() << "," << getName() << "聊天池中==>" << sponsor->getId() << ",("
		<< sponsor->getName() << ")邀请了" << invitedMan->getId() << ",(" << invitedMan->getName() << ")进群" << endl;
	addClient(invitedMan,label);
}

RLabel* ChatPool::createLabel(const int myGrade,const string & labelName,
	int labelGrade, set<Client*>* labelGroup) {
	if (myGrade < labelGrade) {
		cout << "ChatPool::createLabel中不允许创建高于自身等级的标签" << endl;
		return nullptr;
	} else if(hasClients(labelGroup)){
		RLabel* label = new RLabel(labelName, labelGrade,labelGroup);
		labels.insert(label);
		return label;
	} else {
		cout << "该用户组不全或是都不在聊天池中" << endl;
		return nullptr;
	}
}

RLabel * ChatPool::createLabel(const int myGrade, const string & labelName, int labelGrade) {
	if (myGrade < labelGrade) {
		cout << "ChatPool::createLabel中不允许创建高于自身等级的标签" << endl;
		cout << "用户等级：" << myGrade << ",标签等级：" << labelGrade << endl;
		return nullptr;
	} else {
		RLabel* label = new RLabel(labelName, labelGrade);
		labels.insert(label);
		return label;
	}
}

bool ChatPool::setRatingLabel(const int myGrade,RLabel * label, set<Client*>* labelGroup) {
	if (myGrade < label->getGrade()) {
		cout << "ChatPool::setRatingLabel中不允许设置比自身等级高的标签群体" << endl;
		cout << "用户等级：" << myGrade << ",标签等级：" << label->getGrade() << endl;
		return false;
	}
	//确认聊天池中有该标签，拥有该用户群
	set<RLabel*>::iterator it = labels.find(label);
	if (it != labels.end() && hasClients(labelGroup)) {
		(*it)->clients = labelGroup;
		return true;
	} else {
		return false;
	}
}

void ChatPool::addMsg(PoolMsg * msg) {
	GetLocalTime(&sysTime);
	unsigned long time = sysTime.wMonth*tMon +
			sysTime.wDay*tDay + sysTime.wHour*tHour +
			sysTime.wMinute*tMin + sysTime.wSecond*tSec +
			sysTime.wMilliseconds;
	msgs[time] = msg;
	if (msgs.size() > maxSizeOfMsgQ) {
		auto it = --msgs.end();
		delete it->second;
		msgs.erase(it);
	}
}

vector<PoolMsg*>* ChatPool::getMsg(Client * client) {
	map<ChatPool*, ChatPoolAttr*>::iterator it = client->chatPools.find(this);
	vector<PoolMsg*>* vec = new vector<PoolMsg*>;
	if (it != client->chatPools.end()) {
		int grade = it->second->getGrade();		//用户等级
		map<unsigned long, PoolMsg*>::iterator it1 
			= msgs.upper_bound(it->second->getReqTime());
		while (it1 != msgs.end()) {
			if (it1->second->getRecvGrade() == grade) {
				Client* owner = it1->second->getOwner();
				if(client->askParent(this, owner->getId(),
					owner->chatPools[this]->getGrade()))
					vec->push_back(it1->second);
			}
			++it1;
		}
	}
	return vec;
}

bool ChatPool::hasClients(set<Client*>* clients) {
	bool isTrue = true;
	for (auto it : *clients) {
		if (0 == mClients.count(it)) {
			isTrue = false;
			cerr << getId() << "," << getName() << "聊天池验证用户群失败" << endl;
			break;
		}
	}
	return isTrue;
}

unsigned int ChatPool::maxSizeOfMsgQ = 100;

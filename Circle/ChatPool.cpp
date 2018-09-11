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
	//������еı�ǩ
	for (auto it = labels.begin(); it != labels.end(); ++it) {
		delete *it;
	}labels.clear();
	//������е������¼
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
	cout << getId() << "," << getName() << "�������==>" << sponsor->getId() << ",("
		<< sponsor->getName() << ")������" << invitedMan->getId() << ",(" << invitedMan->getName() << ")��Ⱥ" << endl;
	addClient(invitedMan,label);
}

RLabel* ChatPool::createLabel(const int myGrade,const string & labelName,
	int labelGrade, set<Client*>* labelGroup) {
	if (myGrade < labelGrade) {
		cout << "ChatPool::createLabel�в���������������ȼ��ı�ǩ" << endl;
		return nullptr;
	} else if(hasClients(labelGroup)){
		RLabel* label = new RLabel(labelName, labelGrade,labelGroup);
		labels.insert(label);
		return label;
	} else {
		cout << "���û��鲻ȫ���Ƕ������������" << endl;
		return nullptr;
	}
}

RLabel * ChatPool::createLabel(const int myGrade, const string & labelName, int labelGrade) {
	if (myGrade < labelGrade) {
		cout << "ChatPool::createLabel�в���������������ȼ��ı�ǩ" << endl;
		cout << "�û��ȼ���" << myGrade << ",��ǩ�ȼ���" << labelGrade << endl;
		return nullptr;
	} else {
		RLabel* label = new RLabel(labelName, labelGrade);
		labels.insert(label);
		return label;
	}
}

bool ChatPool::setRatingLabel(const int myGrade,RLabel * label, set<Client*>* labelGroup) {
	if (myGrade < label->getGrade()) {
		cout << "ChatPool::setRatingLabel�в��������ñ�����ȼ��ߵı�ǩȺ��" << endl;
		cout << "�û��ȼ���" << myGrade << ",��ǩ�ȼ���" << label->getGrade() << endl;
		return false;
	}
	//ȷ����������иñ�ǩ��ӵ�и��û�Ⱥ
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
		int grade = it->second->getGrade();		//�û��ȼ�
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
			cerr << getId() << "," << getName() << "�������֤�û�Ⱥʧ��" << endl;
			break;
		}
	}
	return isTrue;
}

unsigned int ChatPool::maxSizeOfMsgQ = 100;

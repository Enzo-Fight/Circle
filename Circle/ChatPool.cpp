#include "ChatPool.h"
#include<iostream>
using std::cout;
using std::endl;
using std::cerr;

ChatPool::ChatPool(const string & poolName,unsigned int sponsorId,
	const string& sponsorName) : name(poolName),
	poolId(File::getNewChatPoolId()) {
	mClients[sponsorId] = sponsorName;
}

ChatPool::~ChatPool() {
	//������еı�ǩ
	for (auto it = labels.begin(); it != labels.end(); ++it) {
		delete *it;
	}labels.clear();
}

const unsigned int ChatPool::getId() {
	return poolId;
}

const string & ChatPool::getName() {
	return name;
}

void ChatPool::addClient(unsigned int clientId, const string&
	clientName, RLabel* label) {
	mClients[clientId] = clientName;
	label->addClient(clientId);
}

void ChatPool::delClient(unsigned int clientId, RLabel* label) {
	mClients.erase(clientId);
	label->delClient(clientId);
}

void ChatPool::noticeJoin(unsigned int sponsorId, const string & 
	sponsorName,unsigned int invitedManId, const string &
	invitedManName, RLabel* label) {
	cout << getId() << "," << getName() << "�������==>" << sponsorId << ",("
		<< sponsorName << ")������" << invitedManId << ",(" << invitedManName << ")��Ⱥ" << endl;
	addClient(invitedManId, invitedManName,label);
}

RLabel* ChatPool::createLabel(const int myGrade,const string & labelName,
	int labelGrade, set<unsigned int>* labelGroup) {
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

bool ChatPool::setRatingLabel(const int myGrade,RLabel * label, set<unsigned int>* labelGroup) {
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

bool ChatPool::hasClients(set<unsigned int>* clients) {
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


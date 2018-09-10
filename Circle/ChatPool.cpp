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
	//清除所有的标签
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
	cout << getId() << "," << getName() << "聊天池中==>" << sponsorId << ",("
		<< sponsorName << ")邀请了" << invitedManId << ",(" << invitedManName << ")进群" << endl;
	addClient(invitedManId, invitedManName,label);
}

RLabel* ChatPool::createLabel(const int myGrade,const string & labelName,
	int labelGrade, set<unsigned int>* labelGroup) {
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

bool ChatPool::setRatingLabel(const int myGrade,RLabel * label, set<unsigned int>* labelGroup) {
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

bool ChatPool::hasClients(set<unsigned int>* clients) {
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


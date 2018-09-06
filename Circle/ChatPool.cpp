#include "ChatPool.h"

ChatPool::ChatPool(const string & poolName) : name(poolName), poolId(File::getNewChatPoolId()) {
}

ChatPool::~ChatPool() {
}

RLabel* ChatPool::createLabel(unsigned int clientId,int myGrade, const string & labelName, int labelGrade, vector<Client&>& labelGroup) {
	
	return new RLabel(labelName, labelGrade);;
}

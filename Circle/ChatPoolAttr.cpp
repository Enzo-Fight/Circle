#include "ChatPoolAttr.h"
#include"ChatPool.h"

ChatPoolAttr::ChatPoolAttr(RLabel * label) : mLabel(label) {
	GetLocalTime(&sysTime);
	cacheTime = sysTime.wMonth*ChatPool::tMon +
		sysTime.wDay*ChatPool::tDay + sysTime.wHour*ChatPool::tHour +
		sysTime.wMinute*ChatPool::tMin + sysTime.wSecond*ChatPool::tSec +
		sysTime.wMilliseconds;
}

ChatPoolAttr::~ChatPoolAttr() {}

int ChatPoolAttr::getGrade() { return mLabel->getGrade(); }

void ChatPoolAttr::addFather(Client * father) {
	fathers.push_back(father);
}

void ChatPoolAttr::addChild(Client * child) {
	children.push_back(child);
}

const unsigned long ChatPoolAttr::getReqTime() {
	return cacheTime;
}

void ChatPoolAttr::setCacheTime(const unsigned int time) {
	cacheTime = time;
}

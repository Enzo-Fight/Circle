#include "ChatPoolAttr.h"

ChatPoolAttr::ChatPoolAttr(RLabel * label) : mLabel(label), cacheTime("00000000") {}

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

void ChatPoolAttr::setCacheTime(const string& time) {
	cacheTime = time;
}

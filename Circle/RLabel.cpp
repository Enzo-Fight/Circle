#include "RLabel.h"

RLabel::RLabel(const string & labelName, int labelGrade)
	: name(labelName), grade(labelGrade) {
}

RLabel::RLabel(const string & labelName, int labelGrade,
	set<unsigned int>* clientGroup) : RLabel(labelName, labelGrade) {
	clients = clientGroup;
}

RLabel::~RLabel() {
}

void RLabel::addClient(unsigned int id) {
	clients->insert(id);
}

void RLabel::delClient(unsigned int id) {
	clients->erase(id);
}

int RLabel::getGrade() {
	return grade;
}

#include "RLabel.h"

RLabel::RLabel(const string & labelName, int labelGrade)
	: name(labelName), grade(labelGrade) {
	clients = new set<unsigned int>;
}

RLabel::RLabel(const string & labelName, int labelGrade,
	set<unsigned int>* clientGroup) : RLabel(labelName, labelGrade) {
	clients = new set<unsigned int>;
	clients = clientGroup;
}

RLabel::~RLabel() {
	delete clients;
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

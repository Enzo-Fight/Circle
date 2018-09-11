#include "RLabel.h"

RLabel::RLabel(const string & labelName, int labelGrade)
	: name(labelName), grade(labelGrade) {
	clients = new set<Client*>;
}

RLabel::RLabel(const string & labelName, int labelGrade,
	set<Client*>* clientGroup) : RLabel(labelName, labelGrade) {
	clients = new set<Client*>;
	clients = clientGroup;
}

RLabel::~RLabel() {
	delete clients;
}

void RLabel::addClient(Client* client) {
	clients->insert(client);
}

void RLabel::delClient(Client* client) {
	clients->erase(client);
}

int RLabel::getGrade() {
	return grade;
}

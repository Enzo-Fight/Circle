#include "People.h"
#include"DB.h"

People::People(string name, string password):name(name),password(password),id(DB::getNewClientId()) {

}

People::~People() {

}

const string & People::getName() { return name; }

const unsigned int People::getId() {
	return id;
}

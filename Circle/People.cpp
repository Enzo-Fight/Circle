#include "People.h"
#include"DB.h"

People::People(string name, string password):name(name),password(password),id(DB::getNewId()) {

}

People::~People() {

}

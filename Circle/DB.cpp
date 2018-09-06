#include "DB.h"

DB & DB::getDB() {
	if (nullptr == sigleton) {
		sigleton = new DB();
	}
	return *sigleton;
}

unsigned int DB::getNewClientId() {
	return getDB().lastClientId++;
}

DB::DB() {
}

DB::~DB() {
}

DB* DB::sigleton = nullptr;
unsigned int DB::lastClientId = 0;

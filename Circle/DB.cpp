#include "DB.h"

DB & DB::getDB() {
	if (nullptr == sigleton) {
		sigleton = new DB();
	}
	return *sigleton;
}

unsigned int DB::getNewId() {
	return getDB().lastId++;
}

DB::DB() {
}

DB::~DB() {
}

DB* DB::sigleton = nullptr;
unsigned int DB::lastId = 1;

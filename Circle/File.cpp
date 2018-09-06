#include "File.h"

File & File::getFile() {
	if (nullptr == sigleton) {
		sigleton = new File();
	}
	return *sigleton;
}

unsigned int File::getNewChatPoolId() {
	return getFile().lastChatPoolId++;
}

File::File() {
}

File::~File() {
}

File* File::sigleton = nullptr;
unsigned int File::lastChatPoolId = 0;

#pragma once

class File {
public:
	static File& getFile();

	static unsigned int getNewChatPoolId();

private:
	File();
	~File();

private:
	static File* sigleton;
	static unsigned int lastChatPoolId;
};

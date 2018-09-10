#pragma once
#include"Msg.h"
#include"RLabel.h"
//聊天池中的信息
class PoolMsg :public Msg{
public:
	PoolMsg(unsigned int sendId,RLabel * recvLabel,string content)
		:mOwnerId(sendId),mRecvLabel(recvLabel),Msg(content){}

	~PoolMsg() {}
	const unsigned int getOwnerId() {
		return mOwnerId;
	}
	const int getRecvGrade() {
		return mRecvLabel->getGrade();
	}
private:
	unsigned int mOwnerId;
	RLabel * mRecvLabel;
};

#pragma once
#include"Msg.h"
#include"RLabel.h"
#include"Client.h"
//������е���Ϣ
class PoolMsg :public Msg{
public:
	PoolMsg(Client* owner,RLabel * recvLabel,string content)
		:mOwner(owner),mRecvLabel(recvLabel),Msg(content){}

	~PoolMsg() {}
	Client* getOwner() {
		return mOwner;
	}
	const int getRecvGrade() {
		return mRecvLabel->getGrade();
	}
private:
	Client * mOwner;
	RLabel * mRecvLabel;
};

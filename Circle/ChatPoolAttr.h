#pragma once
#include"Client.h"
#include"RLabel.h"
//��ͬ�û���ͬһ������еĵȼ���һ�������Բ��ܷ����������
class ChatPoolAttr {
	friend class ChatClient;
	friend class Client;
public:
	//cacheTime �ĳ�ʼ��ʱ������ģ���Ϊϵͳ��ǰʱ��
	ChatPoolAttr(RLabel *label); 
	~ChatPoolAttr();
	int getGrade();

	//���һ�����ڵ�
	void addFather(Client* father);

	//���һ���ӽڵ�
	void addChild(Client* child);

	const string& getReqTime();
private:
	void setCacheTime(unsigned long time);
	vector<Client*>fathers;			//���еĸ��ڵ�
	vector<Client*>children;		//���е��ӽڵ�
	RLabel *mLabel;					//�����ȼ���ǩ
	unsigned long cacheTime;				//���������������ص�ʱ��
};

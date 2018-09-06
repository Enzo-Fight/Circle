#pragma once
#include<string>
#include<queue>
#include<vector>
#include"File.h"
using std::string;
using std::queue;
using std::vector;

class Msg;
class Client;

//rating label �ȼ���ǩ
class RLabel {
public:
	int getGrade() { return grade; }
	friend class ChatPool;
private:
	RLabel(const string& labelName,int labelGrade) : name(labelName), grade(labelGrade){}
	~RLabel() {}

private:
	string name;				//��ǩ��
	int grade;					//�ȼ�
	vector<unsigned int>*group;	//��ǩ������Щ��
};

//�����
class ChatPool {
public:
	ChatPool(const string& poolName);
	~ChatPool();

	RLabel* createLabel(unsigned int clientId, int myGrade, const string& labelName,
		int labelGrade, vector<Client&>&labelGroup);

	string name;
	queue<Msg*>msgs;

private:
	const unsigned int poolId;
	vector<RLabel*>labels;
};

//��ͬ�û���ͬһ������еĵȼ���һ�������Բ��ܷ����������
class ChatPoolAttr {
public:
	ChatPoolAttr() {}
	~ChatPoolAttr() {}

	RLabel *label;				//�����ȼ���ǩ
	char time[8];				//���������������ص�ʱ��
};

#pragma once
#include"People.h"
#include"ChatPool.h"
#include<queue>
#include<map>
#include<list>
#include<set>
using std::queue;
using std::map;
using std::list;
using std::vector;
using std::set;
using std::to_string;

#define MAX_LABEL_NAME_LENGTH 12

class PoolAttr;
class RLabel;

class Client : public People{
public:
	Client(string name, string password,unsigned long long phoneNum);
	~Client();

	bool chat(Client& other,Msg& msg);
	bool noticeMsg(Client& fromUser,Msg& msg);

	//���������Ϊ����
	void addFriend(Client& fri);
	//�����Ϊ����
	void reqAddFri(Client& fri);
	//֪ͨ���������Ƿ�ɹ�
	void noticeAddFri(Client& fri, bool isSuccess);
	bool delFriend(Client& fri);

	void print();

private:
	unsigned long long phoneNum;
	map<unsigned int,queue<const Msg*,list<const Msg*>>*>frisMsgQ;	//���Ѽ�������Ϣ
	static unsigned int maxSizeOfMsgQ;								//˽��������Ϣ�������

	bool _dealMsg(const unsigned int id, Msg* msg,bool isRecv);

public:
	//Ⱥ��
	RLabel * createLabel(const string& labelName);
	bool setRatingLabel(ChatPool* pool, vector<Client*>*clientG, RLabel* label);
	bool addFather(Client* father, ChatPool* chatPool);
	bool addChild(Client* child, ChatPool* chatPool);
	bool sendPoolMsg(ChatPool* chatPool, Msg* msg);
	vector<Msg*>* getPoolMsg(ChatPool* chatPool);

private:
	//Ⱥ������
	set<PoolAttr*> chatPools;	//���ڵ����������
	vector<Client*>fathers;			//���еĸ��ڵ�
	vector<Client*>children;			//���е��ӽڵ�

	bool _addFather(Client* father, ChatPool* chatPool);
	bool _addChild(Client* child, ChatPool* chatPool);
	bool labelIsLegal(Client* child, Client* father, ChatPool* chatPool);
};

struct PoolAttr {
	ChatPool *poolAddr;			//���������
	RLabel *label;				//�����ȼ���ǩ
	char time[8];				//���������������ص�ʱ��
};

//rating label �ȼ���ǩ
class RLabel {
public:
	RLabel(const string& labelName) : name(labelName) {}
	~RLabel() {}
	int getGrade() { return grade; }

private:
	string name;			//��ǩ��
	int grade;				//�ȼ�
	vector<Client*>group;	//��ǩ������Щ��  Client*
};
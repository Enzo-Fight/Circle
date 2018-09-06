#pragma once
#include"People.h"
#include"File.h"
#include<queue>
#include<map>
#include<list>
#include<set>
#include"ChatPool.h"

using std::queue;
using std::map;
using std::list;
using std::vector;
using std::set;
using std::to_string;

#define MAX_LABEL_NAME_LENGTH 12

class Client : public People{

public:
	Client(string name, string password,unsigned long long phoneNum);
	~Client();

/* ˽�� */
public:
	bool chat(Client& other,Msg& msg);
	
	//��Ӻ���
	void addFriend(Client& fri);

	//������Ӻ���
	void reqAddFri(Client& fri);

	//�ص���֪ͨ��Ӻ����Ƿ�ɹ�
	void noticeAddFri(Client& fri, bool isSuccess);

	//ɾ������
	bool delFriend(unsigned int friId);

	//���ԡ�����ӡ�������ѵ���Ϣ���д�С
	void print();

private:
	unsigned long long phoneNum;									//�ֻ�����
	map<unsigned int,queue<const Msg*,list<const Msg*>>*>frisMsgQ;	//���Ѽ�������Ϣ
	static unsigned int maxSizeOfMsgQ;								//˽��������Ϣ�������

	bool _dealMsg(const unsigned int id, Msg* msg,bool isRecv);		//����˽����Ϣ

/* ������ͨ�� */
public:
	//���������
	ChatPool& createChatPool(const string& name);

	//�����ȼ���ǩ
	RLabel* createLabel(ChatPool* chatPool, const string& labelName);

	//����������еĵȼ���ǩ
	bool setRatingLabel(ChatPool* pool, vector<Client*>*clientG, RLabel* label);

	//���һ�����ڵ�
	bool addFather(Client* father, ChatPool* chatPool);

	//���һ���ӽڵ�
	bool addChild(Client* child, ChatPool* chatPool);

	//��������з�һ����Ϣ
	bool sendPoolMsg(ChatPool* chatPool, Msg* msg);

	//��ȡ�������Ϣ
	vector<Msg*>* getPoolMsg(ChatPool* chatPool);

private:
	map<ChatPool*, ChatPoolAttr*> chatPools;	//���ڵ����������
	vector<ChatPool*>mChatPools;				//���ƹܵ������(�ϳɽ�����ʱ�迼�������ӵ���ߵ�����)
	vector<Client*>fathers;						//���еĸ��ڵ�
	vector<Client*>children;					//���е��ӽڵ�

	//�໥��ӽڵ�
	bool _addFather(Client* father, ChatPool* chatPool);
	bool _addChild(Client* child, ChatPool* chatPool);

	//�ж������ڵ��Ƿ���Թ��ɸ��ӹ�ϵ�����ݵȼ���ǩ��
	bool labelIsLegal(Client* child, Client* father, ChatPool* chatPool);
};

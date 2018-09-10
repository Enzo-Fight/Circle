#pragma once
#include"People.h"
#include<queue>
#include<map>
#include<list>
#include<set>

using std::queue;
using std::map;
using std::list;
using std::vector;
using std::set;

#define MAX_LABEL_NAME_LENGTH 12

class ChatPool;
class ChatPoolAttr;
class RLabel;

class Client : public People{

public:
	Client(string name, string password, unsigned long long
		phoneNum) :People(name, password), phoneNum(phoneNum) {
	}
	~Client() {}

/* ˽�� */
public:
	virtual bool chat(Client* other, Msg& msg) = 0;
	virtual bool _dealMsg(const unsigned int id, Msg* msg, bool isRecv) = 0;		//����˽����Ϣ

	//������Ӻ���
	virtual void reqAddFri(Client* fri) = 0;
	virtual void _addFriend(Client* fri) = 0;

	//�ص���֪ͨ��Ӻ����Ƿ�ɹ�
	virtual void noticeAddFri(Client* fri, bool isSuccess) = 0;

	//ɾ������
	virtual bool delFriend(unsigned int friId) = 0;


protected:
	unsigned long long phoneNum;									//�ֻ�����
	map<unsigned int,queue<const Msg*,list<const Msg*>>*>frisMsgQ;	//���Ѽ�������Ϣ
	static unsigned int maxSizeOfMsgQ;								//˽��������Ϣ�������

/* ������ͨ�� */
public:
	//���������
	virtual ChatPool* createChatPool(const string& name) = 0;

	//������������
	virtual void inviteJoinPool(Client* mFriend, ChatPool* chatPool,RLabel* label) = 0;

	//�˳������
	virtual void leavePool(ChatPool* chatPool) = 0;

	virtual void addFather(Client * father, ChatPool* chatPool) = 0;

	virtual void addChild(Client * child,ChatPool* chatPool) = 0;

	//�����ȼ���ǩ
	virtual RLabel* createLabel(ChatPool * chatPool, const string & labelName, const int labelGrade) = 0;

	//����������еĵȼ���ǩ
	virtual Client* setRatingLabel(ChatPool* chatPool, RLabel* label, set<unsigned int>*clientG) = 0;

	//��������з�һ����Ϣ
	virtual bool sendPoolMsg(ChatPool* chatPool, Msg* msg) = 0;

	//��ȡ�������Ϣ
	virtual vector<Msg*>* getPoolMsg(ChatPool* chatPool) = 0;

	//���������
	virtual void _joinPool(Client* somebody, ChatPool* chatPool, RLabel* label) = 0;

	map<ChatPool*, ChatPoolAttr*> chatPools;	//���ڵ����������
	set<ChatPool*>mChatPools;				//���ƹܵ������(�ϳɽ�����ʱ�迼�������ӵ���ߵ�����)
};

#pragma once
#include<string>
#include<set>
using std::string;
using std::set;
//rating label �ȼ���ǩ

class Client;

class RLabel {
	friend class ChatPool;
private:
	RLabel(const string& labelName, int labelGrade);
	RLabel(const string& labelName, int labelGrade, set<Client*>* clientGroup);
	~RLabel();
	void addClient(Client* client);
	void delClient(Client* client);

public:
	int getGrade();

private:
	string name;				//��ǩ��
	int grade;					//�ȼ�
	set<Client*>*clients;		 //��ǩ������Щ��
};
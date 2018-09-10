#pragma once
#include<string>
#include<set>
using std::string;
using std::set;
//rating label �ȼ���ǩ
class RLabel {
	friend class ChatPool;
private:
	RLabel(const string& labelName, int labelGrade);
	RLabel(const string& labelName, int labelGrade, set<unsigned int>* clientGroup);
	~RLabel();
	void addClient(unsigned int id);
	void delClient(unsigned int id);

public:
	int getGrade();

private:
	string name;				//��ǩ��
	int grade;					//�ȼ�
	set<unsigned int>*clients;   //��ǩ������Щ��
};
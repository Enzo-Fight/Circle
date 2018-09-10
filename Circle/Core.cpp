#include<iostream>
#include<vector>
#include<queue>
#include<deque>
#include<list>
#include"PoolMsg.h"
#include"ChatClient.h"
#include"Msg.h"
#include<string>
//#pragma comment(lib,"../")
using namespace std;

ChatClient* generateUser() {
	string name, password;
	unsigned long long phoneNum;
	cin >> name >> password >> phoneNum;
	ChatClient* user = new ChatClient(name, password, phoneNum);
	return user;
}

//enzo sdfa 15626056350
//yiyun sdkfj 15625094606
//shabi sdkj 789456123
void test1_singleChat() {
	cout << "�ֱ��������֣����룬�ֻ�����" << endl;
	ChatClient* user1 = generateUser();
	ChatClient* user2 = generateUser();
	ChatClient* user3 = generateUser();
	vector<Msg*>msgVec;
	int length = 10;
	size_t i;
	for (i = 0; i < length; i++) {
		msgVec.push_back(new Msg("msg" + i));
	}
	user1->reqAddFri(user2);
	i = 0;
	user1->chat(user2, *msgVec.at(i++));
	cout << "user1->user2" << endl;
	cout << "user1" << endl;
	user1->print();
	cout << "user2" << endl;
	user2->print();
	user2->reqAddFri(user3);
	user2->chat(user3, *msgVec.at(i++));
	cout << "user2->user3" << endl;
	cout << "user2" << endl;
	user2->print();
	cout << "user3" << endl;
	user3->print();
	user3->chat(user2, *msgVec.at(i++));
	cout << "user3->user2" << endl;
	cout << "user3" << endl;
	user3->print();
	cout << "user2" << endl;
	user2->print();

	delete user1, user2, user3;
	for (i = 0; i < length; i++) {
		delete msgVec.at(i);
	}
	return;
}

void test2_towerChat() {
	//ģ�������ͨ�ţ�δ�����������ϳɹ��ܣ�
	//(�������Ӧ�ü�¼ÿ����Ϣ��ʱ��)

	//��ʼ��
	const short clientSize = 12;
	ChatClient *clients[clientSize];
	for (int i = 0; i < clientSize; i++) {
		clients[i] = new ChatClient(to_string(i), "s", -i);
	}
	vector<Msg*>msgVec;
	size_t i, msgSize = 11;
	for (i = 0; i < msgSize; i++) {
		msgVec.push_back(new Msg("msg" + i));
	}

	//������ϵ���������������ݲ��������ϳɣ�
	ChatPool* chatPool = clients[9]->createChatPool("testPool");
	//(1)�趨��ϵ
	clients[9]->addChild(clients[7], chatPool);
	clients[9]->addChild(clients[8], chatPool);
	clients[7]->addChild(clients[6], chatPool);
	clients[8]->addChild(clients[10], chatPool);
	clients[6]->addChild(clients[4], chatPool);
	clients[6]->addChild(clients[5], chatPool);
	clients[11]->addChild(clients[4], chatPool);
	clients[4]->addChild(clients[1], chatPool);
	clients[4]->addChild(clients[2], chatPool);
	clients[5]->addChild(clients[3], chatPool);
	//(2)�趨��ǩ
	RLabel *label_2, *label_3, *label_4, *label_5;
	//set<unsigned int>label_1set, label_2set, label_3set, label_4set, label_5set;
	//label_1set.insert(clients[1]->getId());
	//label_1set.insert(clients[2]->getId());
	//label_1set.insert(clients[3]->getId());
	//label_1set.insert(clients[10]->getId());
	//label_2set.insert(clients[4]->getId());
	//label_2set.insert(clients[5]->getId());
	//label_3set.insert(clients[11]->getId());
	//label_3set.insert(clients[6]->getId());
	//label_4set.insert(clients[7]->getId());
	//label_4set.insert(clients[8]->getId());
	label_2 = clients[9]->createLabel(chatPool, to_string(i), -2);
	label_3 = clients[9]->createLabel(chatPool, to_string(i), -3);
	label_4 = clients[9]->createLabel(chatPool, to_string(i), -4);
	label_5 = clients[9]->createLabel(chatPool, to_string(i), -5);
	if (label_2 == nullptr || label_3 == nullptr
		|| label_4 == nullptr || label_5 == nullptr ) {
		cout << "core�д�����ǩʧ��" << endl;
		return;
	}
	clients[9]->inviteJoinPool(clients[1], chatPool, label_5);
	clients[9]->inviteJoinPool(clients[2], chatPool, label_5);
	clients[9]->inviteJoinPool(clients[3], chatPool, label_5);
	clients[9]->inviteJoinPool(clients[10], chatPool, label_5);
	clients[9]->inviteJoinPool(clients[4], chatPool, label_4);
	clients[9]->inviteJoinPool(clients[5], chatPool, label_4);
	clients[9]->inviteJoinPool(clients[11], chatPool, label_3);
	clients[9]->inviteJoinPool(clients[6], chatPool, label_3);
	clients[9]->inviteJoinPool(clients[7], chatPool, label_2);
	clients[9]->inviteJoinPool(clients[8], chatPool, label_2);
	//clients[9]->setRatingLabel(chatPool, label_5, &label_1set)
	//	->setRatingLabel(chatPool, label_4, &label_2set)
	//	->setRatingLabel(chatPool, label_3, &label_3set)
	//	->setRatingLabel(chatPool, label_2, &label_4set);

	//�¼����û�6������Ϣ���û�1���ڵȼ�
	clients[6]->sendPoolMsg(chatPool, msgVec.at(i++%msgSize));
	//�¼����û�6��1��10��5��7��ѯ�������Ϣ
	clients[6]->getPoolMsg(chatPool);
	clients[1]->getPoolMsg(chatPool);
	clients[10]->getPoolMsg(chatPool);
	clients[5]->getPoolMsg(chatPool);
	clients[7]->getPoolMsg(chatPool);
	for (size_t i = 0; i < msgSize; i++) {
		delete msgVec.back();
		msgVec.pop_back();
	}
}

//enzo1 sdfa 15626056351
//enzo2 sdfa 15626056352
//enzo3 sdfa 15626056353
//enzo4 sdfa 15626056354
//enzo5 sdfa 15626056355
//enzo6 sdfa 15626056356
//enzo7 sdfa 15626056357
//enzo8 sdfa 15626056358
//enzo9 sdfa 15626056359
void test3_towerConnect() {
	//ģ��������ϳɹ���,ȷ���ϳɺ���Ϣ��������ȷ

}

int main(int argc, char* argv[]) {
	std::ios::sync_with_stdio(false);
	//test1_singleChat();
	test2_towerChat();
	//cout << UINT32_MAX << endl;
	//cout << sizeof(unsigned int) << endl;
	system("pause");
	return 0;
}
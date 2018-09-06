#include<iostream>
#include<vector>
#include<queue>
#include<deque>
#include<list>
#include"Client.h"
#include"Msg.h"
#include"ChatPool.h"
#include"PoolMsg.h"
//#pragma comment(lib,"../")
using namespace std;

Client* generateUser() {
	string name, password;
	unsigned long long phoneNum;
	cin >> name >> password >> phoneNum;
	Client* user = new Client(name, password, phoneNum);
	return user;
}

//enzo sdfa 15626056350
//yiyun sdkfj 15625094606
//shabi sdkj 789456123
void test1_singleChat() {
	cout << "分别输入名字，密码，手机号码" << endl;
	Client* user1 = generateUser();
	Client* user2 = generateUser();
	Client* user3 = generateUser();
	vector<Msg*>msgVec;
	int length = 10;
	size_t i;
	for (i = 0; i < length; i++) {
		msgVec.push_back(new Msg("msg" + i));
	}
	user1->addFriend(*user2);
	i = 0;
	user1->chat(*user2, *msgVec.at(i++));
	cout << "user1->user2" << endl;
	cout << "user1" << endl;
	user1->print();
	cout << "user2" << endl;
	user2->print();
	user2->addFriend(*user3);
	user2->chat(*user3, *msgVec.at(i++));
	cout << "user2->user3" << endl;
	cout << "user2" << endl;
	user2->print();
	cout << "user3" << endl;
	user3->print();
	user3->chat(*user2, *msgVec.at(i++));
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

//1 s -1
//2 s -2
//3 s -3
//4 s -4
//5 s -5
//6 s -6
//7 s -7
//8 s -8
//9 s -9
//10 s -10
//11 s -11
void test2_towerChat() {
	//模拟金字塔通信（未包含金字塔合成功能）
	//(聊天池中应该记录每条消息的时间)
	//(每个用户都应该保存访问聊天池时间，访问聊天池时从该时间开始往后遍历消息)
	cout << "分别输入名字，密码，手机号码" << endl;
	const short clientSize = 12;
	Client *clients[clientSize];
	for (int i = 0; i < clientSize; i++) {
		clients[i] = generateUser();
	}

	vector<Msg*>msgVec;
	size_t i;
	for (i = 0; i < 11; i++) {
		msgVec.push_back(new Msg("msg" + i));
	}i = 0;

	ChatPool chatPool;
	
	//建立关系（定死金字塔，暂不考虑塔合成）
	//(1)设定标签
	RLabel* label_1, *label_2, *label_3, *label_4, *label_5;
	vector<Client*>label_1vec, label_2vec, label_3vec, label_4vec, label_5vec ;
	label_1 = clients[9]->createLabel("1");
	label_1vec.push_back(clients[1]);
	label_1vec.push_back(clients[2]);
	label_1vec.push_back(clients[3]);
	label_1vec.push_back(clients[10]);
	if (!clients[9]->setRatingLabel(&chatPool, &label_1vec, label_1)) {
		cout << "用户9创建标签1失败" << endl;
	}
	label_2 = clients[9]->createLabel("2");
	label_2vec.push_back(clients[4]);
	label_2vec.push_back(clients[5]);
	if (!clients[9]->setRatingLabel(&chatPool, &label_2vec, label_2)) {
		cout << "用户9创建标签2失败" << endl;
	}
	label_3 = clients[9]->createLabel("3");
	label_3vec.push_back(clients[11]);
	label_3vec.push_back(clients[6]);
	if (!clients[9]->setRatingLabel(&chatPool, &label_3vec, label_3)) {
		cout << "用户9创建标签3失败" << endl;
	}
	label_4 = clients[9]->createLabel("4");
	label_4vec.push_back(clients[7]);
	label_4vec.push_back(clients[8]);
	if (!clients[9]->setRatingLabel(&chatPool, &label_4vec, label_4)) {
		cout << "用户9创建标签4失败" << endl;
	}
	label_5 = clients[9]->createLabel("5");
	label_5vec.push_back(clients[9]);
	if (!clients[9]->setRatingLabel(&chatPool, &label_5vec, label_5)) {
		cout << "用户9创建标签5失败" << endl;
	}
	//(2)设定关系
	clients[9]->addChild(clients[7],&chatPool);
	clients[9]->addChild(clients[8],&chatPool);
	clients[7]->addChild(clients[6], &chatPool);
	clients[8]->addChild(clients[10], &chatPool);
	clients[6]->addChild(clients[4], &chatPool);
	clients[6]->addChild(clients[5], &chatPool);
	clients[11]->addChild(clients[4], &chatPool);
	clients[4]->addChild(clients[1], &chatPool);
	clients[4]->addChild(clients[2], &chatPool);
	clients[5]->addChild(clients[3], &chatPool);
	//事件：用户6发送消息给用户1所在等级
	clients[6]->sendPoolMsg(&chatPool,msgVec.at(i++));
	//事件：用户6，1，10，5，7查询聊天池信息
	clients[6]->getPoolMsg(&chatPool);
	clients[1]

	delete label_1, label_2, label_3, label_4, label_5;
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
	//模拟金字塔合成功能,确定合成后消息接受仍正确

}

int main(int argc, char* argv[]) {
	std::ios::sync_with_stdio(false);
	//test1_singleChat();
	test2_towerChat();

	system("pause");
	return 0;
}
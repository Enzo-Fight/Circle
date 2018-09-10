//#include "Client.h"
//using std::cerr;
//using std::to_string;
//
//Client::Client(string name, string password, unsigned long long
//	phoneNum) :People(name, password), phoneNum(phoneNum) {}
//
//Client::~Client() {
//	//���˽����Ϣ����
//	for (auto it = frisMsgQ.begin(); it != frisMsgQ.end();++it) {
//		delete it->second;
//	}frisMsgQ.clear();
//
//	//������ƹܵ������
//	for (auto it = mChatPools.begin(); it != mChatPools.end(); ++it) {
//		map<ChatPool*, ChatPoolAttr*>::iterator mapIt = chatPools.find(*it);
//		if (mapIt != chatPools.end()) {
//			delete mapIt->second;
//			chatPools.erase(mapIt);
//		}
//		delete *it;
//	}mChatPools.clear();
//
//	//�����������ص�����
//	for (auto it = chatPools.begin(); it != chatPools.end(); ++it) {
//		delete it->second;
//	}chatPools.clear();
//}
//
///* ˽�� */
//bool Client::chat(Client & toUser, Msg & msg) {
//	if (!_dealMsg(toUser.getId(), &msg, false)) return false;
//	if (!toUser._dealMsg(this->getId(), &msg, true))return false;
//	return true;
//}
//
//void Client::reqAddFri(Client & fri) {
//	if (0 == frisMsgQ.count(fri.getId())) {
//		fri._addFriend(*this);//��ʱ��������Ҫ���������ضԷ�ͬ�����Ľ��
//	} else {
//		cout << fri.getId() << ",���ƣ�" << fri.getName() << "�Ѿ�����ĺ���" << endl;
//	}
//}
//
//void Client::_addFriend(Client & somebody) {
//	//�˴�Ҫ��������Ϣ�����û��ռ䣬�����Ϸ���
//	//�û�����ʱ����ȡ�û��ռ䣬�鿴�Ƿ���Ϣ��ȷ���Ƿ�Ҫ��Ӻ���
//	//ȷ����Ӻ�����ִ������Ĵ���
//	//��ʱ�趨ֻҪ����ͳ�Ϊ����
//	queue<const Msg*, list<const Msg*>>*msgQ = new queue<const Msg*, list<const Msg*>>;
//	frisMsgQ[somebody.getId()] = msgQ;
//	try {
//		somebody.noticeAddFri(*this, true);	//�ص���֪ͨ������
//	} catch (const std::exception& e) {
//		//�ص��쳣��˵��somebodyû�гɹ�����Լ�Ϊ���ѣ����˵�û����Ӻ��ѵ�״̬
//		delete msgQ;
//		frisMsgQ.erase(somebody.getId());
//		cerr << e.what() << "��somebody.noticeAddFri()�ص��쳣�������Ƕ����ڴ治��" << endl;
//	}
//}
//
//void Client::noticeAddFri(Client & fri, bool isSuccess) {
//	if (isSuccess) {
//		queue<const Msg*, list<const Msg*>>*msgQ = new queue<const Msg*, list<const Msg*>>;
//		frisMsgQ[fri.getId()] = msgQ;
//	} else {
//		cout << this->getId() << ",���ƣ�" << this->getName() << "�������"
//			<< fri.getId() << ",���ƣ�" << fri.getName() << "Ϊ����ʧ��" << endl;
//	}
//	//���۳ɹ����Ҫ��֪ͨ����Ϣ�����û��ռ䣬�û�����ʱ����ȡ�û��ռ��е���Ϣ
//}
//
//bool Client::delFriend(unsigned int friId) {
//	if (frisMsgQ.count(friId)) {
//		delete frisMsgQ[friId];
//		if (1 == frisMsgQ.erase(friId))  return true;
//	}
//	return false;
//}
//
//void Client::print() {
//	cout << "----------------------" << endl <<
//		"id:" << getId() << ",���֣�" << getName() << ",�ֻ���" << phoneNum << endl;
//	for (auto msgQIt : frisMsgQ) {
//		cout << msgQIt.first << "_msgQ.size():" << msgQIt.second->size() << "; ";
//	}cout << endl << "----------------------" << endl;
//}
//
//bool Client::_dealMsg(const unsigned int id, Msg * msg, bool isRecv) {
//	if (!frisMsgQ.count(id)) return false;
//	else {
//		queue<const Msg*, list<const Msg*>>*msgQ = frisMsgQ[id];
//		try {
//			if (msgQ->size() == Client::maxSizeOfMsgQ) {
//				msgQ->pop();
//				msgQ->push(msg);
//			} else if (msgQ->size() < Client::maxSizeOfMsgQ) {
//				msgQ->push(msg);
//			} else for (size_t i = 0; i < msgQ->size() - Client::maxSizeOfMsgQ; i++) {
//				msgQ->pop();
//			}
//		} catch (const std::exception& err) {
//			cerr << err.what() << "�û�id:" << getId() << "_" << (isRecv ? "����" : "����")
//				<< "��id:" << id << "��Ϣʱ������Ϣ�����쳣�������Ƕ����ڴ�";
//			return false;
//		}
//		return true;
//	}
//}
//
//unsigned int Client::maxSizeOfMsgQ = 100;
//
///* ������ͨ�� */
//ChatPool* Client::createChatPool(const string& name) {
//	//����������û�����ֻ��һ���û�ӵ���ͷ�������ڴ��Ȩ�ޣ�mChatPools��
//	ChatPool * chatPool = new ChatPool(name,getId(),getName());
//	set<unsigned int>*initG = new set<unsigned int>;
//	initG->insert(getId());
//	RLabel* label = chatPool->createLabel(INT32_MAX, "������ǩ�û�", 0, initG);
//	ChatPoolAttr* chatPoolAttr = new ChatPoolAttr(label);
//	chatPools[chatPool] = chatPoolAttr;
//	mChatPools.push_back(chatPool);
//	return chatPool;
//}
//
//void Client::inviteJoinPool(Client* mFriend, ChatPool * chatPool, RLabel* label) {
//	if (chatPools.count(chatPool) && frisMsgQ.count(mFriend->getId())) {
//		mFriend->_joinPool(this, chatPool,label);	//��ʱ��������Ҫ���������ضԷ�ͬ�����Ľ��
//	} else {
//		cout << "������Ѽ��������ʧ�ܣ��Ƿ�����ػ������˷Ǻ���" << endl;
//	}
//}
//
//void Client::_joinPool(Client* somebody, ChatPool * chatPool, RLabel* label) {
//	//�˴�Ҫ��������Ϣ�����û��ռ䣬�����Ϸ���
//	//�û�����ʱ����ȡ�û��ռ䣬�鿴�Ƿ���Ϣ��ȷ���Ƿ�Ҫ���������
//	//ȷ�ϼ����������ִ������Ĵ���
//	//��ʱ�趨ֻҪ����ͼ��������
//	ChatPoolAttr* attr = new ChatPoolAttr(label);
//	chatPools[chatPool] = attr;
//	chatPool->noticeJoin(somebody->getId(),somebody->getName(),getId(),getName(),label);
//}
//
//void Client::leavePool(ChatPool * chatPool) {
//	//chatPo
//}
//
//RLabel* Client::createLabel(ChatPool * chatPool, const string & labelName,const int labelGrade) {
//	if (chatPools.count(chatPool)) {
//		return chatPool->createLabel(chatPools[chatPool]->getGrade(), labelName, labelGrade);
//	}
//	return nullptr;
//}
//
//Client* Client::setRatingLabel(ChatPool* chatPool, RLabel* label,
//	set<unsigned int>*clientG){
//	if (chatPools.count(chatPool) && chatPool->setRatingLabel(chatPools[chatPool]->mLabel->getGrade(), label, clientG)) {
//		//cout << "���ñ�ǩ�ɹ�" << endl;
//	} else {
//		cout << "���ñ�ǩʧ��" << endl;
//	}
//	return this;
//}
//
////��������غ�Ӧ֪ͨ�û�1���ڵȼ��������û�ǰ����ȡ��Ϣ==��
////������ߣ���ͨ�����緢����Ϣ���û����յ�ȷ����Ϣ������û��е������ʱ��
//bool Client::sendPoolMsg(ChatPool * chatPool, Msg* msg) {
//
//	return false;
//}
//
////��ָ����������л�ȡ��fromWhen�������Ϣ���ɹ�������û��е������ʱ��
//vector<Msg*>* Client::getPoolMsg(ChatPool * chatPool) {
//	return nullptr;
//}

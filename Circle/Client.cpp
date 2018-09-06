#include "Client.h"
using std::cerr;

Client::Client(string name, string password, unsigned long long phoneNum) :People(name, password), phoneNum(phoneNum) {}

Client::~Client() {
	//���˽����Ϣ����
	for (auto it = frisMsgQ.begin(); it != frisMsgQ.end();++it) {
		delete it->second;
	}frisMsgQ.clear();

	//������ƹܵ������
	for (auto it = mChatPools.begin(); it != mChatPools.end(); ++it) {
		delete *it;
	}mChatPools.clear();
}

/* ˽�� */
bool Client::chat(Client & toUser, Msg & msg) {
	if (!_dealMsg(toUser.getId(), &msg, false)) return false;
	if (!toUser._dealMsg(this->getId(), &msg, true))return false;
	return true;
}

void Client::addFriend(Client & fri) {
	if (0 == frisMsgQ.count(fri.getId())) {
		try {
			fri.reqAddFri(*this);//������������Ǻ�ʱ�������޷����ϻ�֪���,�ʲ�Ҫ����������
		} catch (const std::exception&e) {
			cerr << e.what() << "��������Ӻ����쳣�������Ƕ����ڴ治��" << endl;
		}
	} else {
		cout << fri.getId() << ",���ƣ�" << fri.getName() << "�Ѿ�����ĺ���" << endl;
	}
}

void Client::reqAddFri(Client & somebody) {
	//��ʱ�趨ֻҪ����ͳ�Ϊ����
	queue<const Msg*, list<const Msg*>>*msgQ = new queue<const Msg*, list<const Msg*>>;
	frisMsgQ[somebody.getId()] = msgQ;
	try {
		somebody.noticeAddFri(*this, true);	//�ص���֪ͨ������
	} catch (const std::exception& e) {
		frisMsgQ.erase(somebody.getId());
		cerr << e.what() << "���ص�֪ͨ�쳣�������Ƕ����ڴ治��" << endl;
	}
	//tryû���������ڴ˴�ͨ������֪ͨ�û�

}

void Client::noticeAddFri(Client & fri, bool isSuccess) {
	if (isSuccess) {
		queue<const Msg*, list<const Msg*>>*msgQ = new queue<const Msg*, list<const Msg*>>;
		frisMsgQ[fri.getId()] = msgQ;
	} else {
		cout << this->getId() << ",���ƣ�" << this->getName() << "�������"
			<< fri.getId() << ",���ƣ�" << fri.getName() << "Ϊ����ʧ��" << endl;
	}
}

bool Client::delFriend(unsigned int friId) {
	if (frisMsgQ.count(friId)) {
		delete frisMsgQ[friId];
		if (1 == frisMsgQ.erase(friId))  return true;
	}
	return false;
}

void Client::print() {
	cout << "----------------------" << endl <<
		"id:" << getId() << ",���֣�" << getName() << ",�ֻ���" << phoneNum << endl;
	for (auto msgQIt : frisMsgQ) {
		cout << msgQIt.first << "_msgQ.size():" << msgQIt.second->size() << "; ";
	}cout << endl << "----------------------" << endl;
}

bool Client::_dealMsg(const unsigned int id, Msg * msg, bool isRecv) {
	if (!frisMsgQ.count(id)) return false;
	else {
		queue<const Msg*, list<const Msg*>>*msgQ = frisMsgQ[id];
		try {
			if (msgQ->size() == Client::maxSizeOfMsgQ) {
				msgQ->pop();
				msgQ->push(msg);
			} else if (msgQ->size() < Client::maxSizeOfMsgQ) {
				msgQ->push(msg);
			} else for (size_t i = 0; i < msgQ->size() - Client::maxSizeOfMsgQ; i++) {
				msgQ->pop();
			}
		} catch (const std::exception& err) {
			cerr << err.what() << "�û�id:" << getId() << "_" << (isRecv ? "����" : "����")
				<< "��id:" << id << "��Ϣʱ������Ϣ�����쳣�������Ƕ����ڴ�";
			return false;
		}
		return true;
	}
}

unsigned int Client::maxSizeOfMsgQ = 100;

/* ������ͨ�� */
ChatPool& Client::createChatPool(const string& name) {
	//����������û�����ֻ��һ���û�ӵ���ͷ�������ڴ��Ȩ�ޣ�mChatPools��
	ChatPool * chatPool = new ChatPool(name);	
	mChatPools.push_back(chatPool);
	return *chatPool;
}

RLabel* Client::createLabel(ChatPool * chatPool, const string & labelName) {
	//��ǩ�������û���������ع����û������ǩ��Ҫͨ������ؼ�ӹ���
	//RLabel* label = new RLabel(labelName);		//��������ͷ��ڴ�
	//return label;

	//chatPool->createLabel()
	return nullptr;
}

bool Client::setRatingLabel(ChatPool * chatPool, vector<Client*>* clientG, RLabel * label) {

	return false;
}

bool Client::addFather(Client * father, ChatPool* chatPool) {
	//�ҵ����ӵı�ǩ���ж��ӱ�ǩ�ȼ��ȸ���ǩ�ȼ��Ͳſ��໥��ӹ�ϵ
	if (labelIsLegal(this, father, chatPool)
		&& father->_addChild(this, chatPool)) {
		fathers.push_back(father);
		return true;
	}
	return false;
}

bool Client::addChild(Client * child, ChatPool* chatPool) {
	if (labelIsLegal(child, this, chatPool)
		&& child->_addFather(this, chatPool)) {
		children.push_back(child);
		return true;
	}
	return false;
}

//��������غ�Ӧ֪ͨ�û�1���ڵȼ��������û�ǰ����ȡ��Ϣ==��
//������ߣ���ͨ�����緢����Ϣ���û����յ�ȷ����Ϣ������û��е������ʱ��
bool Client::sendPoolMsg(ChatPool * chatPool, Msg* msg) {
	return false;
}

//��ָ����������л�ȡ��fromWhen�������Ϣ���ɹ�������û��е������ʱ��
vector<Msg*>* Client::getPoolMsg(ChatPool * chatPool) {
	return nullptr;
}

bool Client::_addFather(Client * father, ChatPool* chatPool) {
	fathers.push_back(father);
	return false;
}

bool Client::_addChild(Client * child, ChatPool* chatPool) {
	children.push_back(child);
	return false;
}

bool Client::labelIsLegal(Client * child, Client * father, ChatPool* chatPool) {
	if (child->chatPools.count(chatPool) && father->chatPools.count(chatPool)) {
		return child->chatPools[chatPool]->label->getGrade() < father->chatPools[chatPool]->label->getGrade();
	} else {
		cerr << "��֤(" << child->getName() << ",id:" << child->getId() << ")��("
			<< father->getName() << ",id:" << father->getId()
			<< "�Ƿ�Ϊ���¹�ϵʱ���ڶ������е��������û���ҵ�ָ���������" << endl;
		return false;
	}
}

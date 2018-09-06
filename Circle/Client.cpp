#include "Client.h"
using std::cerr;

Client::Client(string name, string password, unsigned long long phoneNum) :People(name, password), phoneNum(phoneNum) {
}

Client::~Client() {
}

bool Client::chat(Client & toUser, Msg & msg) {
	if (!_dealMsg(toUser.getId(), &msg, false)) return false;
	if (!toUser.noticeMsg(*this, msg))return false;
	return true;
}

bool Client::noticeMsg(Client& fromUser, Msg & msg) {
	return _dealMsg(fromUser.getId(), &msg, true);
}

void Client::addFriend(Client & fri) {
	fri.reqAddFri(*this);//������������Ǻ�ʱ�������޷����ϻ�֪���,�ʲ�Ҫ����������
}

void Client::reqAddFri(Client & somebody) {
	//��ʱ�趨ֻҪ����ͳ�Ϊ����
	queue<const Msg*, list<const Msg*>>*msgQ = new queue<const Msg*, list<const Msg*>>;
	frisMsgQ[somebody.getId()] = msgQ;
	somebody.noticeAddFri(*this, true);
}

void Client::noticeAddFri(Client & fri, bool isSuccess) {
	if (isSuccess) {
		queue<const Msg*, list<const Msg*>>*msgQ = new queue<const Msg*, list<const Msg*>>;
		frisMsgQ[fri.getId()] = msgQ;
	} else {

	}
}

bool Client::delFriend(Client & fri) {
	if(1 == frisMsgQ.erase(fri.getId())) return true;
	else return false;
}

void Client::print() {
	cout << "----------------------"<< endl <<
		"id:" << getId() << ",���֣�" << getName() << ",�ֻ���" << phoneNum << endl;
	for (auto msgQIt : frisMsgQ) {
		cout << msgQIt.first << "_msgQ.size():" << msgQIt.second->size() << "; ";
	}cout << endl << "----------------------" << endl;
}

bool Client::_dealMsg(const unsigned int id, Msg * msg,bool isRecv) {
	if (!frisMsgQ.count(id)) return false;
	else {
		queue<const Msg*,list<const Msg*>>*msgQ = frisMsgQ[id];
		if (msgQ->size() == Client::maxSizeOfMsgQ) {
			msgQ->pop();
			msgQ->push(msg);
		} else if (msgQ->size() < Client::maxSizeOfMsgQ) {
			try {
				msgQ->push(msg);
			} catch (const std::exception& err) {
				string str;
				cerr << err.what() << "�û�id:" << getId()
					<< "_" << (isRecv ? "����": "����" )
					<< "��id:" <<id << "��Ϣʱ������Ϣ�����쳣";
				return false;
			}
		} else for (size_t i = 0; i < msgQ->size() - Client::maxSizeOfMsgQ; i++) {
			msgQ->pop();
		}
		return true;
	}
}

RLabel * Client::createLabel(const string & labelName) {
	if (labelName.size() > MAX_LABEL_NAME_LENGTH) {
		cerr << getName() << "������ǩʧ�ܣ���ǩ������(" << labelName.size() << ")����ǩ���������Ϊ��" << MAX_LABEL_NAME_LENGTH << endl;
		return nullptr;
	} else {
		return new RLabel(labelName);
	}
}

bool Client::setRatingLabel(ChatPool * pool, vector<Client*>* clientG, RLabel * label) {
	return false;
}

bool Client::addFather(Client * father, ChatPool* chatPool) {
	//�ҵ����ӵı�ǩ���ж��ӱ�ǩ�ȼ��ȸ���ǩ�ȼ��Ͳſ��໥��ӹ�ϵ
	if (labelIsLegal(this,father,chatPool)
		&& father->_addChild(this, chatPool)) {
		fathers.push_back(father);
		return true;
	}
	return false;
}

bool Client::addChild(Client * child, ChatPool* chatPool) {
	if (labelIsLegal(child,this,chatPool)
		&& child->_addFather(this, chatPool)) {
		children.push_back(child);
		return true;
	}
	return false;
}

//��������غ�Ӧ֪ͨ�û�1���ڵȼ��������û�ǰ����ȡ��Ϣ==��
//������ߣ���ͨ�����緢����Ϣ���û����յ�ȷ����Ϣ������û��е������ʱ��
bool Client::sendPoolMsg(ChatPool * chatPool,Msg* msg) {
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
	RLabel *childLabel = nullptr, *fatherLabel = nullptr;

	/*for (auto poolAttr : child->chatPools) {
		if (poolAttr->poolAddr == chatPool)childLabel = poolAttr->label;
	}
	for (auto poolAttr : father->chatPools) {
		if (poolAttr->poolAddr == chatPool)fatherLabel = poolAttr->label;
	}*/
	return fatherLabel && childLabel 
		&& childLabel->getGrade() < fatherLabel->getGrade();
}

unsigned int Client::maxSizeOfMsgQ = 100;

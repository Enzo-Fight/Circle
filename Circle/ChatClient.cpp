#pragma once
#include"File.h"
#include"ChatPool.h"
#include"ChatPoolAttr.h"
#include"Client.h"

using std::cout;
using std::cerr;
using std::endl;

class ChatClient : public Client {
public:
	ChatClient(string name, string password, unsigned long long phoneNum)
		: Client(name, password, phoneNum) {
	}

	~ChatClient() {
		//���˽����Ϣ����
		for (auto it = frisMsgQ.begin(); it != frisMsgQ.end(); ++it) {
			delete it->second;
		}frisMsgQ.clear();

		//������ƹܵ������
		for (auto it = mChatPools.begin(); it != mChatPools.end(); ++it) {
			map<ChatPool*, ChatPoolAttr*>::iterator mapIt = chatPools.find(*it);
			if (mapIt != chatPools.end()) {
				delete mapIt->second;
				chatPools.erase(mapIt);
			}
			delete *it;
		}mChatPools.clear();

		//�����������ص�����
		for (auto it = chatPools.begin(); it != chatPools.end(); ++it) {
			delete it->second;
		}chatPools.clear();
	}

	/* ˽�� */
public:
	bool chat(Client * toUser, Msg & msg) {
		toUser->getId();
		if (!_dealMsg(toUser->getId(), &msg, false)) return false;
		if (!toUser->_dealMsg(this->getId(), &msg, true))return false;
		return true;
	}

	void reqAddFri(Client* fri) {
		if (0 == frisMsgQ.count(fri->getId()) && getId() != fri->getId()) {
			fri->_addFriend(this);//��ʱ��������Ҫ���������ضԷ�ͬ�����Ľ��
		}
	}

	void noticeAddFri(Client* fri, bool isSuccess) {
		if (isSuccess) {
			queue<const Msg*, list<const Msg*>>*msgQ = new queue<const Msg*, list<const Msg*>>;
			frisMsgQ[fri->getId()] = msgQ;
		} else {
			cout << this->getId() << ",���ƣ�" << this->getName() << "�������"
				<< fri->getId() << ",���ƣ�" << fri->getName() << "Ϊ����ʧ��" << endl;
		}
		//���۳ɹ����Ҫ��֪ͨ����Ϣ�����û��ռ䣬�û�����ʱ����ȡ�û��ռ��е���Ϣ
	}

	bool delFriend(unsigned int friId) {
		if (frisMsgQ.count(friId)) {
			delete frisMsgQ[friId];
			if (1 == frisMsgQ.erase(friId))  return true;
		}
		return false;
	}

	void print() {
		cout << "----------------------" << endl <<
			"id:" << getId() << ",���֣�" << getName() << ",�ֻ���" << phoneNum << endl;
		for (auto msgQIt : frisMsgQ) {
			cout << msgQIt.first << "_msgQ.size():" << msgQIt.second->size() << "; ";
		}cout << endl << "----------------------" << endl;
	}

private:
	void _addFriend(Client* somebody) {
		//�˴�Ҫ��������Ϣ�����û��ռ䣬�����Ϸ���
		//�û�����ʱ����ȡ�û��ռ䣬�鿴�Ƿ���Ϣ��ȷ���Ƿ�Ҫ��Ӻ���
		//ȷ����Ӻ�����ִ������Ĵ���
		//��ʱ�趨ֻҪ����ͳ�Ϊ����
		queue<const Msg*, list<const Msg*>>*msgQ = new queue<const Msg*, list<const Msg*>>;
		frisMsgQ[somebody->getId()] = msgQ;
		try {
			somebody->noticeAddFri(this, true);	//�ص���֪ͨ������
		} catch (const std::exception& e) {
			//�ص��쳣��˵��somebodyû�гɹ�����Լ�Ϊ���ѣ����˵�û����Ӻ��ѵ�״̬
			delete msgQ;
			frisMsgQ.erase(somebody->getId());
			cerr << e.what() << "��somebody.noticeAddFri()�ص��쳣�������Ƕ����ڴ治��" << endl;
		}
	}

	bool _dealMsg(const unsigned int id, Msg * msg, bool isRecv) {
		if (!frisMsgQ.count(id)) return false;
		else {
			queue<const Msg*, list<const Msg*>>*msgQ = frisMsgQ[id];
			if (msgQ == nullptr) {
				cout << "msgQ Ϊ��ָ��" << endl;
			}
			try {
				if (msgQ->size() == maxSizeOfMsgQ) {
					msgQ->pop();
					msgQ->push(msg);
				} else if (msgQ->size() < maxSizeOfMsgQ) {
					msgQ->push(msg);
				} else for (size_t i = 0; i < msgQ->size() - maxSizeOfMsgQ; i++) {
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

	/* ������ͨ�� */
public:
	ChatPool * createChatPool(const string& name) {
		//����������û�����ֻ��һ���û�ӵ���ͷ�������ڴ��Ȩ�ޣ�mChatPools��
		ChatPool * chatPool = new ChatPool(name, this);
		set<Client*>*initG = new set<Client*>;
		initG->insert(this);
		RLabel* label = chatPool->createLabel(INT32_MAX, "������ǩ�û�", 0, initG);
		ChatPoolAttr* chatPoolAttr = new ChatPoolAttr(label);
		chatPools[chatPool] = chatPoolAttr;
		mChatPools.insert(chatPool);
		return chatPool;
	}

	void inviteJoinPool(Client* mFriend, ChatPool * chatPool, RLabel* label) {
		if (chatPools.count(chatPool) && frisMsgQ.count(mFriend->getId())) {
			mFriend->_joinPool(this, chatPool, label);	//��ʱ��������Ҫ���������ضԷ�ͬ�����Ľ��
		} else {
			cout << "������Ѽ��������ʧ�ܣ��Ƿ�����ػ������˷Ǻ���" << endl;
		}
	}

	void leavePool(ChatPool * chatPool) {
		chatPool->delClient(this, chatPools[chatPool]->mLabel);
		chatPools.erase(chatPool);
		mChatPools.erase(chatPool);
	}

	void addFather(Client * father, ChatPool* chatPool) {
		map<ChatPool*, ChatPoolAttr*>::iterator childIt, fatherIt;
		childIt = chatPools.find(chatPool);
		fatherIt = father->chatPools.find(chatPool);
		//�Ƿ�ӵ�и������
		if (childIt != chatPools.end() && fatherIt != father->chatPools.end()) {
			if (childIt->second->getGrade() < fatherIt->second->getGrade()) {
				childIt->second->addFather(father);
				fatherIt->second->addChild(this);
			} else {
				cerr << "��֤(" << getName() << ",id:" << getId() << ")��("
					<< father->getName() << ",id:" << father->getId()
					<< ")�Ƿ�Ϊ���¹�ϵʱ�����߲��������¼���ϵ" << endl;
			}
		} else {
			cerr << "��֤(" << getName() << ",id:" << getId() << ")��("
				<< father->getName() << ",id:" << father->getId()
				<< ")�Ƿ�Ϊ���¹�ϵʱ���ڶ������е��������û���ҵ�ָ���������" << endl;
		}
	}

	void addChild(Client * child, ChatPool* chatPool) {
		map<ChatPool*, ChatPoolAttr*>::iterator childIt, fatherIt;
		childIt = child->chatPools.find(chatPool);
		fatherIt = chatPools.find(chatPool);
		//�Ƿ�ӵ�и������
		if (childIt != child->chatPools.end() && fatherIt != chatPools.end()) {
			if (childIt->second->getGrade() < fatherIt->second->getGrade()) {
				childIt->second->addFather(this);
				fatherIt->second->addChild(child);
			} else {
				cerr << "��֤(" << child->getName() << ",id:" << child->getId() << ")��("
					<< getName() << ",id:" << getId()
					<< ")�Ƿ�Ϊ���¹�ϵʱ�����߲��������¼���ϵ" << endl;
			}
		} else {
			cerr << "��֤(" << child->getName() << ",id:" << child->getId() << ")��("
				<< getName() << ",id:" << getId()
				<< ")�Ƿ�Ϊ���¹�ϵʱ���ڶ������е��������û���ҵ�ָ���������" << endl;
		}
	}

	RLabel* createLabel(ChatPool * chatPool, const string & labelName, const int labelGrade) {
		if (chatPools.count(chatPool)) {
			return chatPool->createLabel(chatPools[chatPool]->getGrade(), labelName, labelGrade);
		}
		return nullptr;
	}

	Client* setRatingLabel(ChatPool* chatPool, RLabel* label,
		set<Client*>*clientG) {
		map<ChatPool*, ChatPoolAttr*>::iterator it = chatPools.find(chatPool);
		if (it != chatPools.end()) {
			chatPool->setRatingLabel(it->second->mLabel->getGrade(), label, clientG);
			//cout << "���ñ�ǩ�ɹ�" << endl;
		} else {
			cout << "���ñ�ǩʧ��" << endl;
		}
		return this;
	}

	//��������غ�Ӧ֪ͨ�û�1���ڵȼ��������û�ǰ����ȡ��Ϣ==��
	//������ߣ���ͨ�����緢����Ϣ���û����յ�ȷ����Ϣ������û��е������ʱ��
	void sendPoolMsg(ChatPool * chatPool, Msg* msg, RLabel * label) {
		PoolMsg * poolMsg = new PoolMsg(this, label, msg->getContent());
		chatPool->addMsg(poolMsg);
	}

	//��ָ����������л�ȡ��fromWhen�������Ϣ���ɹ�������û��е������ʱ��
	vector<PoolMsg*>* getPoolMsg(ChatPool * chatPool) {
		return chatPool->getMsg(this);
	}

	//�˴��Ƚ�Ӧ����LRU�㷨���û����ʵ�id���浽��ǰ�û���
	bool askParent(ChatPool*chatPool, const unsigned int id, const int grade) {
		map<ChatPool*, ChatPoolAttr*>::iterator attrIt = chatPools.find(chatPool);
		if (attrIt != chatPools.end()) {
			if (grade > attrIt->second->getGrade()) {
				for (auto it : attrIt->second->fathers) {
					//ֻҪ��һ�����׻ظ���id���汲���ͷ���true
					if (it->askParent(chatPool, id, grade)) {
						return true;
					}
				}
				return false;		//���еĸ��׶�����false
			} else if (id == getId()) {
				return true;		//�ҵ���id
			} else {
				return false;		//û���ҵ���id
			}
		} else {
			cerr << "���ַǷ��û����û�����������У������id��"
				<< chatPool->getId() << ",Ѱ��id��" + id << "id�ȼ���"
				<< grade << ",��ǰ�û�id��" << getId() << ",�û�����"
				<< getName() << endl;
			return false;
		}
	}

private:
	void _joinPool(Client* somebody, ChatPool * chatPool, RLabel* label) {
		//�˴�Ҫ��������Ϣ�����û��ռ䣬�����Ϸ���
		//�û�����ʱ����ȡ�û��ռ䣬�鿴�Ƿ���Ϣ��ȷ���Ƿ�Ҫ���������
		//ȷ�ϼ����������ִ������Ĵ���
		//��ʱ�趨ֻҪ����ͼ��������
		ChatPoolAttr* attr = new ChatPoolAttr(label);
		chatPools[chatPool] = attr;
		chatPool->noticeJoin(somebody, this, label);
	}
};

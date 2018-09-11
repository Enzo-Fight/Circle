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
		//清除私聊消息队列
		for (auto it = frisMsgQ.begin(); it != frisMsgQ.end(); ++it) {
			delete it->second;
		}frisMsgQ.clear();

		//清除所掌管的聊天池
		for (auto it = mChatPools.begin(); it != mChatPools.end(); ++it) {
			map<ChatPool*, ChatPoolAttr*>::iterator mapIt = chatPools.find(*it);
			if (mapIt != chatPools.end()) {
				delete mapIt->second;
				chatPools.erase(mapIt);
			}
			delete *it;
		}mChatPools.clear();

		//清除所在聊天池的属性
		for (auto it = chatPools.begin(); it != chatPools.end(); ++it) {
			delete it->second;
		}chatPools.clear();
	}

	/* 私聊 */
public:
	bool chat(Client * toUser, Msg & msg) {
		toUser->getId();
		if (!_dealMsg(toUser->getId(), &msg, false)) return false;
		if (!toUser->_dealMsg(this->getId(), &msg, true))return false;
		return true;
	}

	void reqAddFri(Client* fri) {
		if (0 == frisMsgQ.count(fri->getId()) && getId() != fri->getId()) {
			fri->_addFriend(this);//耗时动作，不要求立即返回对方同意与否的结果
		}
	}

	void noticeAddFri(Client* fri, bool isSuccess) {
		if (isSuccess) {
			queue<const Msg*, list<const Msg*>>*msgQ = new queue<const Msg*, list<const Msg*>>;
			frisMsgQ[fri->getId()] = msgQ;
		} else {
			cout << this->getId() << ",名称：" << this->getName() << "请求添加"
				<< fri->getId() << ",名称：" << fri->getName() << "为好友失败" << endl;
		}
		//无论成功与否要将通知的信息放在用户空间，用户上线时，获取用户空间中的信息
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
			"id:" << getId() << ",名字：" << getName() << ",手机：" << phoneNum << endl;
		for (auto msgQIt : frisMsgQ) {
			cout << msgQIt.first << "_msgQ.size():" << msgQIt.second->size() << "; ";
		}cout << endl << "----------------------" << endl;
	}

private:
	void _addFriend(Client* somebody) {
		//此处要将请求信息放在用户空间，并马上返回
		//用户在线时，获取用户空间，查看是否信息，确认是否要添加好友
		//确认添加好友再执行下面的代码
		//暂时设定只要申请就成为好友
		queue<const Msg*, list<const Msg*>>*msgQ = new queue<const Msg*, list<const Msg*>>;
		frisMsgQ[somebody->getId()] = msgQ;
		try {
			somebody->noticeAddFri(this, true);	//回调，通知请求结果
		} catch (const std::exception& e) {
			//回调异常，说明somebody没有成功添加自己为好友，回退到没有添加好友的状态
			delete msgQ;
			frisMsgQ.erase(somebody->getId());
			cerr << e.what() << "，somebody.noticeAddFri()回调异常，可能是堆区内存不足" << endl;
		}
	}

	bool _dealMsg(const unsigned int id, Msg * msg, bool isRecv) {
		if (!frisMsgQ.count(id)) return false;
		else {
			queue<const Msg*, list<const Msg*>>*msgQ = frisMsgQ[id];
			if (msgQ == nullptr) {
				cout << "msgQ 为空指针" << endl;
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
				cerr << err.what() << "用户id:" << getId() << "_" << (isRecv ? "接收" : "发送")
					<< "到id:" << id << "消息时插入消息队列异常，可能是堆区内存";
				return false;
			}
			return true;
		}
	}

	/* 金字塔通信 */
public:
	ChatPool * createChatPool(const string& name) {
		//聊天池属于用户，但只有一个用户拥有释放聊天池内存的权限（mChatPools）
		ChatPool * chatPool = new ChatPool(name, this);
		set<Client*>*initG = new set<Client*>;
		initG->insert(this);
		RLabel* label = chatPool->createLabel(INT32_MAX, "初创标签用户", 0, initG);
		ChatPoolAttr* chatPoolAttr = new ChatPoolAttr(label);
		chatPools[chatPool] = chatPoolAttr;
		mChatPools.insert(chatPool);
		return chatPool;
	}

	void inviteJoinPool(Client* mFriend, ChatPool * chatPool, RLabel* label) {
		if (chatPools.count(chatPool) && frisMsgQ.count(mFriend->getId())) {
			mFriend->_joinPool(this, chatPool, label);	//耗时动作，不要求立即返回对方同意与否的结果
		} else {
			cout << "邀请好友加入聊天池失败，非法聊天池或受邀人非好友" << endl;
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
		//是否拥有该聊天池
		if (childIt != chatPools.end() && fatherIt != father->chatPools.end()) {
			if (childIt->second->getGrade() < fatherIt->second->getGrade()) {
				childIt->second->addFather(father);
				fatherIt->second->addChild(this);
			} else {
				cerr << "验证(" << getName() << ",id:" << getId() << ")和("
					<< father->getName() << ",id:" << father->getId()
					<< ")是否为上下关系时，二者不满足上下级关系" << endl;
			}
		} else {
			cerr << "验证(" << getName() << ",id:" << getId() << ")和("
				<< father->getName() << ",id:" << father->getId()
				<< ")是否为上下关系时，在二者所有的聊天池中没有找到指定的聊天池" << endl;
		}
	}

	void addChild(Client * child, ChatPool* chatPool) {
		map<ChatPool*, ChatPoolAttr*>::iterator childIt, fatherIt;
		childIt = child->chatPools.find(chatPool);
		fatherIt = chatPools.find(chatPool);
		//是否拥有该聊天池
		if (childIt != child->chatPools.end() && fatherIt != chatPools.end()) {
			if (childIt->second->getGrade() < fatherIt->second->getGrade()) {
				childIt->second->addFather(this);
				fatherIt->second->addChild(child);
			} else {
				cerr << "验证(" << child->getName() << ",id:" << child->getId() << ")和("
					<< getName() << ",id:" << getId()
					<< ")是否为上下关系时，二者不满足上下级关系" << endl;
			}
		} else {
			cerr << "验证(" << child->getName() << ",id:" << child->getId() << ")和("
				<< getName() << ",id:" << getId()
				<< ")是否为上下关系时，在二者所有的聊天池中没有找到指定的聊天池" << endl;
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
			//cout << "设置标签成功" << endl;
		} else {
			cout << "设置标签失败" << endl;
		}
		return this;
	}

	//发到聊天池后应通知用户1所在等级的所有用户前来获取消息==》
	//如果在线，则通过网络发送消息给用户，收到确认信息后更新用户中的聊天池时间
	void sendPoolMsg(ChatPool * chatPool, Msg* msg, RLabel * label) {
		PoolMsg * poolMsg = new PoolMsg(this, label, msg->getContent());
		chatPool->addMsg(poolMsg);
	}

	//从指定的聊天池中获取从fromWhen至今的信息，成功则更新用户中的聊天池时间
	vector<PoolMsg*>* getPoolMsg(ChatPool * chatPool) {
		return chatPool->getMsg(this);
	}

	//此处比较应采用LRU算法将用户访问的id缓存到当前用户中
	bool askParent(ChatPool*chatPool, const unsigned int id, const int grade) {
		map<ChatPool*, ChatPoolAttr*>::iterator attrIt = chatPools.find(chatPool);
		if (attrIt != chatPools.end()) {
			if (grade > attrIt->second->getGrade()) {
				for (auto it : attrIt->second->fathers) {
					//只要有一个父亲回复该id是祖辈，就返回true
					if (it->askParent(chatPool, id, grade)) {
						return true;
					}
				}
				return false;		//所有的父亲都返回false
			} else if (id == getId()) {
				return true;		//找到该id
			} else {
				return false;		//没有找到该id
			}
		} else {
			cerr << "出现非法用户，用户不在聊天池中，聊天池id："
				<< chatPool->getId() << ",寻找id：" + id << "id等级："
				<< grade << ",当前用户id：" << getId() << ",用户名："
				<< getName() << endl;
			return false;
		}
	}

private:
	void _joinPool(Client* somebody, ChatPool * chatPool, RLabel* label) {
		//此处要将请求信息放在用户空间，并马上返回
		//用户在线时，获取用户空间，查看是否信息，确认是否要加入聊天池
		//确认加入聊天池再执行下面的代码
		//暂时设定只要申请就加入聊天池
		ChatPoolAttr* attr = new ChatPoolAttr(label);
		chatPools[chatPool] = attr;
		chatPool->noticeJoin(somebody, this, label);
	}
};

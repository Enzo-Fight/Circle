#include "Client.h"
using std::cerr;

Client::Client(string name, string password, unsigned long long phoneNum) :People(name, password), phoneNum(phoneNum) {}

Client::~Client() {
	//清除私聊消息队列
	for (auto it = frisMsgQ.begin(); it != frisMsgQ.end();++it) {
		delete it->second;
	}frisMsgQ.clear();

	//清除所掌管的聊天池
	for (auto it = mChatPools.begin(); it != mChatPools.end(); ++it) {
		delete *it;
	}mChatPools.clear();
}

/* 私聊 */
bool Client::chat(Client & toUser, Msg & msg) {
	if (!_dealMsg(toUser.getId(), &msg, false)) return false;
	if (!toUser._dealMsg(this->getId(), &msg, true))return false;
	return true;
}

void Client::addFriend(Client & fri) {
	if (0 == frisMsgQ.count(fri.getId())) {
		try {
			fri.reqAddFri(*this);//由于请求好友是耗时动作，无法马上获知结果,故不要求立即返回
		} catch (const std::exception&e) {
			cerr << e.what() << "，请求添加好友异常，可能是堆区内存不足" << endl;
		}
	} else {
		cout << fri.getId() << ",名称：" << fri.getName() << "已经是你的好友" << endl;
	}
}

void Client::reqAddFri(Client & somebody) {
	//暂时设定只要申请就成为好友
	queue<const Msg*, list<const Msg*>>*msgQ = new queue<const Msg*, list<const Msg*>>;
	frisMsgQ[somebody.getId()] = msgQ;
	try {
		somebody.noticeAddFri(*this, true);	//回调，通知请求结果
	} catch (const std::exception& e) {
		frisMsgQ.erase(somebody.getId());
		cerr << e.what() << "，回调通知异常，可能是堆区内存不足" << endl;
	}
	//try没有问题再在此处通过网络通知用户

}

void Client::noticeAddFri(Client & fri, bool isSuccess) {
	if (isSuccess) {
		queue<const Msg*, list<const Msg*>>*msgQ = new queue<const Msg*, list<const Msg*>>;
		frisMsgQ[fri.getId()] = msgQ;
	} else {
		cout << this->getId() << ",名称：" << this->getName() << "请求添加"
			<< fri.getId() << ",名称：" << fri.getName() << "为好友失败" << endl;
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
		"id:" << getId() << ",名字：" << getName() << ",手机：" << phoneNum << endl;
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
			cerr << err.what() << "用户id:" << getId() << "_" << (isRecv ? "接收" : "发送")
				<< "到id:" << id << "消息时插入消息队列异常，可能是堆区内存";
			return false;
		}
		return true;
	}
}

unsigned int Client::maxSizeOfMsgQ = 100;

/* 金字塔通信 */
ChatPool& Client::createChatPool(const string& name) {
	//聊天池属于用户，但只有一个用户拥有释放聊天池内存的权限（mChatPools）
	ChatPool * chatPool = new ChatPool(name);	
	mChatPools.push_back(chatPool);
	return *chatPool;
}

RLabel* Client::createLabel(ChatPool * chatPool, const string & labelName) {
	//标签不属于用户，由聊天池管理，用户管理标签需要通过聊天池间接管理
	//RLabel* label = new RLabel(labelName);		//由聊天池释放内存
	//return label;

	//chatPool->createLabel()
	return nullptr;
}

bool Client::setRatingLabel(ChatPool * chatPool, vector<Client*>* clientG, RLabel * label) {

	return false;
}

bool Client::addFather(Client * father, ChatPool* chatPool) {
	//找到父子的标签，判断子标签等级比父标签等级低才可相互添加关系
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

//发到聊天池后应通知用户1所在等级的所有用户前来获取消息==》
//如果在线，则通过网络发送消息给用户，收到确认信息后更新用户中的聊天池时间
bool Client::sendPoolMsg(ChatPool * chatPool, Msg* msg) {
	return false;
}

//从指定的聊天池中获取从fromWhen至今的信息，成功则更新用户中的聊天池时间
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
		cerr << "验证(" << child->getName() << ",id:" << child->getId() << ")和("
			<< father->getName() << ",id:" << father->getId()
			<< "是否为上下关系时，在二者所有的聊天池中没有找到指定的聊天池" << endl;
		return false;
	}
}

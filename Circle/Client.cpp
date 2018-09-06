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
	fri.reqAddFri(*this);//由于请求好友是耗时动作，无法马上获知结果,故不要求立即返回
}

void Client::reqAddFri(Client & somebody) {
	//暂时设定只要申请就成为好友
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
		"id:" << getId() << ",名字：" << getName() << ",手机：" << phoneNum << endl;
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
				cerr << err.what() << "用户id:" << getId()
					<< "_" << (isRecv ? "接收": "发送" )
					<< "到id:" <<id << "消息时插入消息队列异常";
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
		cerr << getName() << "创建标签失败，标签名过长(" << labelName.size() << ")，标签名长度最大为：" << MAX_LABEL_NAME_LENGTH << endl;
		return nullptr;
	} else {
		return new RLabel(labelName);
	}
}

bool Client::setRatingLabel(ChatPool * pool, vector<Client*>* clientG, RLabel * label) {
	return false;
}

bool Client::addFather(Client * father, ChatPool* chatPool) {
	//找到父子的标签，判断子标签等级比父标签等级低才可相互添加关系
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

//发到聊天池后应通知用户1所在等级的所有用户前来获取消息==》
//如果在线，则通过网络发送消息给用户，收到确认信息后更新用户中的聊天池时间
bool Client::sendPoolMsg(ChatPool * chatPool,Msg* msg) {
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

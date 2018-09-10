//#include "Client.h"
//using std::cerr;
//using std::to_string;
//
//Client::Client(string name, string password, unsigned long long
//	phoneNum) :People(name, password), phoneNum(phoneNum) {}
//
//Client::~Client() {
//	//清除私聊消息队列
//	for (auto it = frisMsgQ.begin(); it != frisMsgQ.end();++it) {
//		delete it->second;
//	}frisMsgQ.clear();
//
//	//清除所掌管的聊天池
//	for (auto it = mChatPools.begin(); it != mChatPools.end(); ++it) {
//		map<ChatPool*, ChatPoolAttr*>::iterator mapIt = chatPools.find(*it);
//		if (mapIt != chatPools.end()) {
//			delete mapIt->second;
//			chatPools.erase(mapIt);
//		}
//		delete *it;
//	}mChatPools.clear();
//
//	//清除所在聊天池的属性
//	for (auto it = chatPools.begin(); it != chatPools.end(); ++it) {
//		delete it->second;
//	}chatPools.clear();
//}
//
///* 私聊 */
//bool Client::chat(Client & toUser, Msg & msg) {
//	if (!_dealMsg(toUser.getId(), &msg, false)) return false;
//	if (!toUser._dealMsg(this->getId(), &msg, true))return false;
//	return true;
//}
//
//void Client::reqAddFri(Client & fri) {
//	if (0 == frisMsgQ.count(fri.getId())) {
//		fri._addFriend(*this);//耗时动作，不要求立即返回对方同意与否的结果
//	} else {
//		cout << fri.getId() << ",名称：" << fri.getName() << "已经是你的好友" << endl;
//	}
//}
//
//void Client::_addFriend(Client & somebody) {
//	//此处要将请求信息放在用户空间，并马上返回
//	//用户在线时，获取用户空间，查看是否信息，确认是否要添加好友
//	//确认添加好友再执行下面的代码
//	//暂时设定只要申请就成为好友
//	queue<const Msg*, list<const Msg*>>*msgQ = new queue<const Msg*, list<const Msg*>>;
//	frisMsgQ[somebody.getId()] = msgQ;
//	try {
//		somebody.noticeAddFri(*this, true);	//回调，通知请求结果
//	} catch (const std::exception& e) {
//		//回调异常，说明somebody没有成功添加自己为好友，回退到没有添加好友的状态
//		delete msgQ;
//		frisMsgQ.erase(somebody.getId());
//		cerr << e.what() << "，somebody.noticeAddFri()回调异常，可能是堆区内存不足" << endl;
//	}
//}
//
//void Client::noticeAddFri(Client & fri, bool isSuccess) {
//	if (isSuccess) {
//		queue<const Msg*, list<const Msg*>>*msgQ = new queue<const Msg*, list<const Msg*>>;
//		frisMsgQ[fri.getId()] = msgQ;
//	} else {
//		cout << this->getId() << ",名称：" << this->getName() << "请求添加"
//			<< fri.getId() << ",名称：" << fri.getName() << "为好友失败" << endl;
//	}
//	//无论成功与否要将通知的信息放在用户空间，用户上线时，获取用户空间中的信息
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
//		"id:" << getId() << ",名字：" << getName() << ",手机：" << phoneNum << endl;
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
//			cerr << err.what() << "用户id:" << getId() << "_" << (isRecv ? "接收" : "发送")
//				<< "到id:" << id << "消息时插入消息队列异常，可能是堆区内存";
//			return false;
//		}
//		return true;
//	}
//}
//
//unsigned int Client::maxSizeOfMsgQ = 100;
//
///* 金字塔通信 */
//ChatPool* Client::createChatPool(const string& name) {
//	//聊天池属于用户，但只有一个用户拥有释放聊天池内存的权限（mChatPools）
//	ChatPool * chatPool = new ChatPool(name,getId(),getName());
//	set<unsigned int>*initG = new set<unsigned int>;
//	initG->insert(getId());
//	RLabel* label = chatPool->createLabel(INT32_MAX, "初创标签用户", 0, initG);
//	ChatPoolAttr* chatPoolAttr = new ChatPoolAttr(label);
//	chatPools[chatPool] = chatPoolAttr;
//	mChatPools.push_back(chatPool);
//	return chatPool;
//}
//
//void Client::inviteJoinPool(Client* mFriend, ChatPool * chatPool, RLabel* label) {
//	if (chatPools.count(chatPool) && frisMsgQ.count(mFriend->getId())) {
//		mFriend->_joinPool(this, chatPool,label);	//耗时动作，不要求立即返回对方同意与否的结果
//	} else {
//		cout << "邀请好友加入聊天池失败，非法聊天池或受邀人非好友" << endl;
//	}
//}
//
//void Client::_joinPool(Client* somebody, ChatPool * chatPool, RLabel* label) {
//	//此处要将请求信息放在用户空间，并马上返回
//	//用户在线时，获取用户空间，查看是否信息，确认是否要加入聊天池
//	//确认加入聊天池再执行下面的代码
//	//暂时设定只要申请就加入聊天池
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
//		//cout << "设置标签成功" << endl;
//	} else {
//		cout << "设置标签失败" << endl;
//	}
//	return this;
//}
//
////发到聊天池后应通知用户1所在等级的所有用户前来获取消息==》
////如果在线，则通过网络发送消息给用户，收到确认信息后更新用户中的聊天池时间
//bool Client::sendPoolMsg(ChatPool * chatPool, Msg* msg) {
//
//	return false;
//}
//
////从指定的聊天池中获取从fromWhen至今的信息，成功则更新用户中的聊天池时间
//vector<Msg*>* Client::getPoolMsg(ChatPool * chatPool) {
//	return nullptr;
//}

//
//  rpcMsg.cpp
//  testC
//
//  Created by 陈帅 on 2018/11/05.
//

#include "rpcMsg.hpp"
#include "singleServer.hpp"



void rpcMsg::login(const ::example::LoginInfo& loginInfo,
		cxx::function<void(int32_t ret_code, const ::example::LoginRet& loginRet)>& rsp)
{
	int32_t roleID = loginInfo.roleid();
	int32_t roomID = loginInfo.roomid();

	// 处理请求时记录请求的来源，用于反向RPC调用，不过注意这个handle是可能失效的
	_server->bindRole2Handle(roleID);
	int32_t status = gm->roleLogin(roleID, roomID);

	std::cout << "receive rpc loginInfo: " << roleID << " + " << roomID << " = " << status << std::endl;
	::example::LoginRet loginRet;
	loginRet.set_status(status);
	rsp(pebble::kRPC_SUCCESS, loginRet);
}

void rpcMsg::add(const ::example::CalRequest& request,
		cxx::function<void(int32_t ret_code, const ::example::CalResponse& response)>& rsp)
{
	int32_t a = request.a();
	int32_t b = request.b();
	int32_t c = a + b;
	std::cout << "receive rpc request: " << a << " + " << b << " = " << c << std::endl;
	::example::CalResponse response;
	response.set_c(c);
	rsp(pebble::kRPC_SUCCESS, response);
}

void rpcMsg::modifyStatus(const ::example::StatusRequest& statusReq,
		cxx::function<void(int32_t ret_code, const ::example::StatusResponse& ret)>& rsp)
{
	int32_t cmd = statusReq.cmd();
	int32_t roleID = _server->getLastMsgRoleID();
	gm->modifyRoleStatus(roleID, cmd);

	int32_t status = 0;

	std::cout << "receive rpc cmd: " << cmd << std::endl;
	::example::StatusResponse ret;
	ret.set_status(status);
	rsp(pebble::kRPC_SUCCESS, ret);
}

void rpcMsg::move(const ::example::moveRequest& moveCMD,
		cxx::function<void(int32_t ret_code, const ::example::StatusResponse& ret)>& rsp)
{
	int32_t dir = moveCMD.direction();
	int32_t roleID = _server->getLastMsgRoleID();
	gm->inputRoleDir(roleID, dir);
}

void rpcMsg::chat(const ::example::chatBroadcast& chatInfo,
		cxx::function<void(int32_t ret_code, const ::example::commonResponse& ret)>& rsp)
{
	string mySaid = chatInfo.said();
	int32_t roleID = _server->getLastMsgRoleID();

	//消息层直接拿到同房玩家列表，转发，效率更高
	pebble::RpcHead head;
	::example::chatBroadcast sendChat;
	sendChat.set_said(mySaid);
	int __size = sendChat.ByteSize();
	pebble::PebbleRpc* rpc = _server->getBinaryRpc();
	uint8_t *__buff = rpc->GetBuffer(__size);
	sendChat.SerializeToArray(__buff, __size);

	list<int32_t> roleIDList = gm->getBroadcastRoleIDList(roleID);
	list<int32_t>::iterator iter;
	for(iter = roleIDList.begin(); iter != roleIDList.end(); iter++)
	{
		_server->sendMsg(*iter, __buff, __size);
	}

	int32_t status = 0;

	std::cout << "receive rpc role: " << roleID << "say: " << mySaid << std::endl;
	::example::commonResponse ret;
	ret.set_status(status);
	rsp(pebble::kRPC_SUCCESS, ret);
}

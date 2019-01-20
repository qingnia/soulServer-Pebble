//
//  rpcMsg.cpp
//  testC
//
//  Created by 陈帅 on 2018/11/05.
//

#include "rpcMsg.hpp"
#include "singleServer.hpp"
#include "../gameEntity/gameMgr.h"

void rpcMsg::init()
{
        this->gm = gameMgr::getGameMgr();
}

void rpcMsg::login(const ::example::LoginInfo& loginInfo,
		cxx::function<void(int32_t ret_code, const ::example::playersInfo& loginRet)>& rsp)
{
	int32_t roleID = loginInfo.roleid();
	int32_t roomID = loginInfo.roomid();

	std::cout << "receive rpc loginInfo: " << roleID << " + " << roomID << std::endl;
	// 处理请求时记录请求的来源，用于反向RPC调用，不过注意这个handle是可能失效的
	_server->bindRole2Handle(roleID);
	std::cout << "bind success" << roomID << std::endl;
	list<playerBaseInfo> baseInfos;
	int32_t roomHolder;
	retStatus status = gm->roleLogin(roleID, roomID, baseInfos, roomHolder);
	std::cout << "login success, status:" << status << "roomID:" << roomID << std::endl;

	//登陆的返回做一次修改，如果登陆成功，就将当前房间内的所有玩家同步给客户端
	
	::example::playersInfo loginRet;
	::example::commonResponse* cr;
	cr = loginRet.mutable_cr();
	cr->set_status(status);
	if (status == rsSuccess)
	{
		loginRet.set_roomholder(roomHolder);
		list<playerBaseInfo>::iterator iter;
		for(iter = baseInfos.begin(); iter != baseInfos.end(); iter++)
		{
			::example::baseInfo* playerBaseInfo = loginRet.add_baseinfos();
			if (!playerBaseInfo)
			{
				cout << "登陆单播协议组织失败";
				return;
			}
			playerBaseInfo->set_roleid(iter->roleID);
			playerBaseInfo->set_name(iter->name);
			playerBaseInfo->set_status(iter->ps);
		}
	}
	rsp(pebble::kRPC_SUCCESS, loginRet);

	//将新玩家进入的消息通知到其他人
	::example::playersInfo newLoginBroad;
	::example::commonResponse* newCR = newLoginBroad.mutable_cr();
	newCR->set_status(rsSuccess);
	::example::baseInfo* newBaseInfo = newLoginBroad.add_baseinfos();
	if (!newBaseInfo)
	{
cout << "登陆广播协议组织失败";
		return;
	}
	newBaseInfo->set_roleid(roleID);
	newBaseInfo->set_name("");
	newBaseInfo->set_status(psEnter);
	list<int32_t> roleIDList = gm->getBroadcastRoleIDList(roleID);
	int __size = newLoginBroad.ByteSize();
	pebble::PebbleRpc* rpc = _server->getBinaryRpc();
	uint8_t *__buff = rpc->GetBuffer(__size);
	newLoginBroad.SerializeToArray(__buff, __size);
	roleIDList.pop_back();
	_server->broadcastMsg("newJoin", roleIDList, __buff, __size);
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

void rpcMsg::modifyStatus(const ::example::StatusReceive& statusReq,
		cxx::function<void(int32_t ret_code, const ::example::commonResponse& ret)>& rsp)
{
	int32_t cmd = statusReq.cmd();
	int32_t roleID = _server->getLastMsgRoleID();
	std::cout << "receive rpc cmd: " << cmd << std::endl;
	int32_t actionRoleID = 0;
	retStatus rs = gm->modifyRoleStatus(roleID, cmd, actionRoleID);
	std::cout << "receive rpc cmd: " << cmd << ", rs:" << rs << std::endl;

	//如果状态修改成功了，要向所有人更新，如果是start，要通知所有人开始游戏
	if (rs == rsSuccess)
	{
		std::cout << "status modify send to all Action:" << actionRoleID << std::endl;
		//消息层直接拿到同房玩家列表，转发，效率更高
		::example::statusBroadcast sendStatus;
		sendStatus.set_roleid(roleID);
		sendStatus.set_cmd(cmd);
		if (actionRoleID > 0)
		{
			sendStatus.set_actionroleid(actionRoleID);
		}
		int __size = sendStatus.ByteSize();
		pebble::PebbleRpc* rpc = _server->getBinaryRpc();
		uint8_t *__buff = rpc->GetBuffer(__size);
		sendStatus.SerializeToArray(__buff, __size);

		list<int32_t> roleIDList = gm->getBroadcastRoleIDList(roleID);
		list<int32_t>::iterator iter;
		for (iter = roleIDList.begin(); iter != roleIDList.end(); iter++)
		{
			_server->sendMsg("statusBroad", *iter, __buff, __size);
		}
	}

	::example::commonResponse ret;
	ret.set_status(rs);
	rsp(pebble::kRPC_SUCCESS, ret);
}

void rpcMsg::move(const ::example::moveRequest& moveCMD,
		cxx::function<void(int32_t ret_code, const ::example::commonResponse& ret)>& rsp)
{
	int32_t dir = moveCMD.direction();
	int32_t roleID = _server->getLastMsgRoleID();
	::example::moveBroadcast sendMove;
	retStatus rs = gm->inputRoleDir(roleID, dir, sendMove);
	if (rs == rsSuccess)
	{
		sendMove.set_roleid(roleID);
		sendMove.set_cmd(dir);
		std::cout << roleID << "move send to all, dir:" << dir << endl;

		int __size = sendMove.ByteSize();
		pebble::PebbleRpc* rpc = _server->getBinaryRpc();
		uint8_t *__buff = rpc->GetBuffer(__size);
		sendMove.SerializeToArray(__buff, __size);
		list<int32_t> roleIDList = gm->getBroadcastRoleIDList(roleID);
		_server->broadcastMsg("moveBroad", roleIDList, __buff, __size);
	}

	::example::commonResponse ret;
	ret.set_status(rs);
	rsp(pebble::kRPC_SUCCESS, ret);
}


void rpcMsg::chat(const ::example::chatReceive& chatInfo,
		cxx::function<void(int32_t ret_code, const ::example::commonResponse& ret)>& rsp)
{
	string mySaid = chatInfo.said();
	int32_t roleID = _server->getLastMsgRoleID();

	std::cout << "receive rpc role: " << roleID << "say: " << mySaid << std::endl;

	//消息层直接拿到同房玩家列表，转发，效率更高
	::example::chatBroadcast sendChat;
	sendChat.set_roleid(roleID);
	sendChat.set_said(mySaid);
	int __size = sendChat.ByteSize();
	pebble::PebbleRpc* rpc = _server->getBinaryRpc();
	uint8_t *__buff = rpc->GetBuffer(__size);
	sendChat.SerializeToArray(__buff, __size);

	std::cout << "ssssssssssssssss " << mySaid << std::endl;
	list<int32_t> roleIDList = gm->getBroadcastRoleIDList(roleID);
	_server->broadcastMsg("chatBroad", roleIDList, __buff, __size);
	int32_t status = 0;
	::example::commonResponse ret;
	ret.set_status(status);
	rsp(pebble::kRPC_SUCCESS, ret);
}

void rpcMsg::chatBroad(const ::example::chatBroadcast& chatInfo,
        cxx::function<void(int32_t ret_code, const ::example::commonResponse& ret)>& rsp)
{
}

void rpcMsg::attack(const ::example::attackRequest& attackCMD,
		cxx::function<void(int32_t ret_code, const ::example::commonResponse& ret)>& rsp)
{
	int32_t targetID = attackCMD.targetid();
	int32_t type = attackCMD.type();
	int32_t roleID = _server->getLastMsgRoleID();
	std::cout << "receive rpc role: " << roleID << "attack: " << targetID << std::endl;

	::example::attackBroadcast attackBroad;
	retStatus rs = gm->attack(roleID, targetID, type, attackBroad);
	if (rs == rsSuccess)
	{
		attackBroad.set_roleid(roleID);
		attackBroad.set_targetid(targetID);
		attackBroad.set_type(type);
		std::cout << roleID << "attack send to all," << "role:" << roleID <<  "target:" << targetID << endl;

		int __size = attackBroad.ByteSize();
		pebble::PebbleRpc* rpc = _server->getBinaryRpc();
		uint8_t *__buff = rpc->GetBuffer(__size);
		attackBroad.SerializeToArray(__buff, __size);
		list<int32_t> roleIDList = gm->getBroadcastRoleIDList(roleID);
		_server->broadcastMsg("attackBroad", roleIDList, __buff, __size);
	}

	::example::commonResponse ret;
	ret.set_status(rs);
	rsp(pebble::kRPC_SUCCESS, ret);
}


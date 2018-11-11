﻿//
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
	retStatus status = gm->roleLogin(roleID, roomID, baseInfos);
	std::cout << "login success, status:" << status << "roomID:" << roomID << std::endl;

	//登陆的返回做一次修改，如果登陆成功，就将当前房间内的所有玩家同步给客户端
	
	::example::playersInfo loginRet;
	::example::commonResponse* cr;
	cr = loginRet.mutable_cr();
	cr->set_status(status);
	if (status == rsSuccess)
	{
		list<playerBaseInfo>::iterator iter;
		for(iter = baseInfos.begin(); iter != baseInfos.end(); iter++)
		{
			::example::baseInfo* playerBaseInfo = loginRet.add_baseinfos();
			if (!playerBaseInfo)
			{
				return;
			}
			playerBaseInfo->set_roleid(iter->roleID);
			playerBaseInfo->set_name(iter->name);
			playerBaseInfo->set_status(iter->ps);
		}
	}
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

void rpcMsg::modifyStatus(const ::example::StatusReceive& statusReq,
		cxx::function<void(int32_t ret_code, const ::example::commonResponse& ret)>& rsp)
{
	int32_t cmd = statusReq.cmd();
	int32_t roleID = _server->getLastMsgRoleID();
	retStatus rs = gm->modifyRoleStatus(roleID, cmd);
	std::cout << "receive rpc cmd: " << cmd << ", rs:" << rs << std::endl;

	//如果状态修改成功了，要向所有人更新，如果是start，要通知所有人开始游戏
	if (rs == rsSuccess)
	{
		std::cout << "status modify send to all " << std::endl;
		//消息层直接拿到同房玩家列表，转发，效率更高
		::example::statusBroadcast sendStatus;
		sendStatus.set_roleid(roleID);
		sendStatus.set_cmd(cmd);
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
	gm->inputRoleDir(roleID, dir);
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

for(int i = 0; i < (int)__size; i++)
{
printf("______send Chat:---%d\n", int(__buff[i]));
}

	std::cout << "ssssssssssssssss " << mySaid << std::endl;

	list<int32_t> roleIDList = gm->getBroadcastRoleIDList(roleID);
	list<int32_t>::iterator iter;
	for(iter = roleIDList.begin(); iter != roleIDList.end(); iter++)
	{
		_server->sendMsg("chatBroad", *iter, __buff, __size);
	}

	int32_t status = 0;
	::example::commonResponse ret;
	ret.set_status(status);
	rsp(pebble::kRPC_SUCCESS, ret);
}

void rpcMsg::chatBroad(const ::example::chatBroadcast& chatInfo,
        cxx::function<void(int32_t ret_code, const ::example::commonResponse& ret)>& rsp)
{
}

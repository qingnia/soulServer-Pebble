/*
 * Tencent is pleased to support the open source community by making Pebble available.
 * Copyright (C) 2016 THL A29 Limited, a Tencent company. All rights reserved.
 * Licensed under the MIT License (the "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 * http://opensource.org/licenses/MIT
 * Unless required by applicable law or agreed to in writing, software distributed under the License
 * is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or implied. See the License for the specific language governing permissions and limitations under
 * the License.
 *
 */

#include <cstdio>
#include <iostream>

#include "singleServer.hpp"
#include "server/pebble_server.h"
#include "rpcMsg.hpp"

#define ASSERT_EQ(expected, actual) \
    if ((expected) != (actual)) { \
        fprintf(stderr, "(%s:%d)(%s) %d != %d\n", __FILE__, __LINE__, __FUNCTION__, (expected), (actual)); \
        exit(1); \
    }

void usage() {
    std::cout << "usage   : ./server url" << std::endl
              << "default : url = tcp://127.0.0.1:9000" << std::endl;
}

singleServer* singleServer::ss = new singleServer();

singleServer::singleServer()
{
    // 初始化PebbleServer
    server = pebble::PebbleServer();
}

singleServer::~singleServer()
{

}

singleServer* singleServer::getSingleServer()
{
    return ss;
}

int32_t singleServer::getLastMsgRoleID()
{
    int64_t handle = this->server->GetLastMessageInfo->_remote_handle;
    return this->getRoleIDByHandle(handle);
}

void singleServer::bindRole2Handle(int32_t roleID, int64_t handle)
{
    roleID2Handle[roleID] = handle;
    handle2RoleID[handle] = roleID;
}

int64_t singleServer::getHandleByRoleID(int32_t roleID);
{
    map<int64_t, int32_t>::iterator iter = this->roleID2Handle.find(handle);
	if (iter == this->roleID2Handle.end())
	{
		return -1;
	}	
	return iter->second;
}

int32_t singleServer::getRoleIDByHandle(int64_t handle)
{
	map<int64_t, int32_t>::iterator iter = this->handle2RoleID.find(handle);
	if (iter == this->handle2RoleID.end())
	{
		return -1;
	}
	return iter->second;
}

int singleServer::setRecQueue(list< map<string, string> >* queue)
{
    this->recQueue = queue;
    return 0;
}

int singleServer::setRecMutex(mutex* mt)
{
	this->recMutex = mt;
	return 0;
}

int singleServer::setRetQueue(list< map<string, string> >* queue)
{
	this->retQueue = queue;
	return 0;
}

int singleServer::setRetMutex(mutex* mt)
{
	this->retMutex = mt;
	return 0;
}

int singleServer::saveMsg(map<string, string> newMsg)
{
    recQueue->push_back(newMsg);
    return 0;
}

int32_t singleServer::sendMsg(int32_t roleID, int8_t* buff, int32_t buff_len)
{
    int64_t handle = this->getHandleByRoleID(roleID);
    pebble::PebbleRpc* rpc = server.GetPebbleRpc(ProtocolType.kPEBBLE_RPC_BINARY);
    RpcHead head = RpcHead();
    int64_t handle = this->getHandleByRoleID(*roleIter);
    rpc->SendRequestSync(handle, head, msgIter->buff, msgIter->buff_len, msgIter->on_rsp, 10);
}

int32_t singleServer::On1sTimeout()
{
    std::lock_guard<std::mutex> guard(*retMutex);
    pebble::PebbleRpc* rpc = server.GetPebbleRpc(ProtocolType.kPEBBLE_RPC_BINARY);
    list< string, map<string, needSaveMsg*> >::iterator msgIter;
    for(msgIter = this->retQueue->begin(); msgIter != this->retQueue->end(); msgIter++)
    {
        list<int32_t>::iterator roleIter;
        for(roleIter = msgIter->roleIDList.begin(); roleIter != msgIter->roleIDList.end(); roleIter++)
        {
            RpcHead head = RpcHead();
            int64_t handle = this->getHandleByRoleID(*roleIter);
            rpc->SendRequestSync(handle, head, msgIter->buff, msgIter->buff_len, msgIter->on_rsp, 10);
        }
    }
    return 0;
}

void singleServer::setSendMsgTimer()
{
    // 创建定时器，定时向客户端推消息
    server.GetTimer()->StartTimer(1000, cxx::bind(On1sTimeout));
}

int singleServer::serverStart()
{
    int argc;
    const char** argv;
    usage();

    std::string url("tcp://0.0.0.0:9000");
    if (argc > 1) url.assign(argv[1]);

    int ret = server.Init();
    ASSERT_EQ(0, ret);

    // 添加传输
    int64_t handle = server.Bind(url);
    ASSERT_EQ(true, handle >= 0);

    // 获取PebbleRpc实例
    pebble::PebbleRpc* rpc = server.GetPebbleRpc(pebble::kPEBBLE_RPC_PROTOBUF);
    ASSERT_EQ(true, rpc != NULL);

    // 指定通道的处理器
    server.Attach(handle, rpc);

    // 添加服务
    rpcMsg rs;
    rs._server = &server;

    ret = rpc->AddService(&rs);
    ASSERT_EQ(0, ret);

    server.Serve();

    return 0;
}




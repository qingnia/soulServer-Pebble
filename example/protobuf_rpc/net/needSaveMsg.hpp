//
//  pebbleServer.hpp
//  testC
//
//  Created by 陈帅 on 2018/10/17.
//
#ifndef needSaveMsg_hpp
#define needSaveMsg_hpp

#include "rpcMsg.rpc.pb.h"
#include "src/framework/rpc.h"

class needSaveMsg
{
public:
    needSaveMsg(list<int32_t> roleIDList, int64_t handle, uint8_t* buff, uint32_t buff_len)
    {
        this->handle = handle;
        this->buff = buff;
        this->buff_len = buff_len;
        this->roleIDList = roleIDList;
    }
    ~needSaveMsg()
    {
        free(buff);
    }
    list<int32_t> roleIDList;
	int64_t handle;
	const pebble::RpcHead& rpc_head;
	const uint8_t* buff;
	uint32_t buff_len;
	const pebble::OnRpcResponse& on_rsp;
	int32_t timeout_ms;
};

#endif
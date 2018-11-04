//
//  pebbleServer.hpp
//  testC
//
//  Created by 陈帅 on 2018/11/04.
//
#ifndef needSaveMsg_hpp
#define needSaveMsg_hpp

#include "example/protobuf_rpc/net/rpcMsg.rpc.pb.h"
#include "src/framework/rpc.h"
#include <string>
#include <list>

using namespace std;
class needSaveMsg
{
public:
    needSaveMsg(string function, list<int32_t> roleIDList, uint8_t* buff, uint32_t buff_len)
    {
        this->roleIDList = roleIDList;
        this->buff = buff;
        this->buff_len = buff_len;
        this->roleIDList = roleIDList;
    }
    ~needSaveMsg()
    {
        free(buff);
    }
	string function;
    list<int32_t> roleIDList;
    //pebble::RpcHead& rpc_head;
    uint8_t* buff;
    uint32_t buff_len;
    //pebble::OnRpcResponse& on_rsp;
    int32_t timeout_ms;
};

#endif

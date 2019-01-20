//
//  rpcMsg.hpp
//  testC
//
//  Created by 陈帅 on 2018/10/12.
//
#ifndef rpcMsg_hpp
#define rpcMsg_hpp

#include <cstdio>
#include <iostream>

#include "example/protobuf_rpc/net/rpcMsg.rpc.pb.h"
#include "server/pebble_server.h"
#include "needSaveMsg.hpp"

class singleServer;
class gameMgr;
// Calculator服务接口的实现
class rpcMsg : public ::example::rpcMsgServerInterface {
public:
    //singleServer* ss;
    //pebble::PebbleServer* _server;
	singleServer* _server;
    gameMgr* gm;
    rpcMsg() : _server() {
	this->init();
        //_server = singleServer::getSingleServer();
    }
	void init();
    virtual ~rpcMsg() {}

	//每个service都会被拆分成一个process，一个return，整理在__rpcMsgSkeleton类
	//__rpcMsgSkeleton把process函数注册进pebble_rpc里面

	//pebble_server->pebble_rpc->rpc->ProcessRequestImp
	//process的调用代码：(it->second)(buff, buff_len, rsp);
	//cxx::function<int32_t(int32_t, const uint8_t*, uint32_t)> rsp = cxx::bind( // NOLINT
	//	&IRpc::SendResponse, this, session->m_session_id,
	//	cxx::placeholders::_1, cxx::placeholders::_2, cxx::placeholders::_3);

	//process会把return注册进服务器的实际执行函数，即参数的rsp
	//执行流程为：	
	//	1. pebble根据thrift的function_name路由到process函数
	//	2. process把return函数改成回调格式传参，把客户端发过来的消息序列化成protobuf格式，传参，执行login
	//	3. login执行，返回值写回到protobuf结构的引用，执行rsp（实际执行对应的return函数）
	//	4. return参数包括：参数1process的rsp,参数2login的状态码，参数3login的返回protobuf格式buff
	//	5. return函数会申请好返回的__buff内存，再调用rsp(ret_code, __buff, __size)
	//	6. 这个rsp实际是SendResponse(session_id, ret, buff, buff_len)

    virtual void login(const ::example::LoginInfo& loginInfo,
        cxx::function<void(int32_t ret_code, const ::example::playersInfo& loginRet)>& rsp);

    virtual void add(const ::example::CalRequest& request,
        cxx::function<void(int32_t ret_code, const ::example::CalResponse& response)>& rsp);

    virtual void modifyStatus(const ::example::StatusReceive& statusReq,
        cxx::function<void(int32_t ret_code, const ::example::commonResponse& ret)>& rsp);

    virtual void move(const ::example::moveRequest& moveCMD,
        cxx::function<void(int32_t ret_code, const ::example::commonResponse& ret)>& rsp);

    virtual void chat(const ::example::chatReceive& chatInfo,
        cxx::function<void(int32_t ret_code, const ::example::commonResponse& ret)>& rsp);

    virtual void chatBroad(const ::example::chatBroadcast& chatInfo,
        cxx::function<void(int32_t ret_code, const ::example::commonResponse& ret)>& rsp);

    virtual void attack(const ::example::attackRequest& attackCMD,
        cxx::function<void(int32_t ret_code, const ::example::commonResponse& ret)>& rsp);
};
#endif

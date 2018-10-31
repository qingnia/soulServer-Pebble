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
#include "../gameEntity/gameMgr.h"
//#include "example/protobuf_rpc/gameEntity/gameMgr.hpp"
#include "singleServer.hpp"

// Calculator服务接口的实现
class rpcMsg : public ::example::rpcMsgServerInterface {
public:
    singleServer* ss;
    pebble::PebbleServer* _server;
    gameMgr* gm;
    rpcMsg() : _server(NULL) {
        gm = gameMgr::getGameMgr();
        ss = singleServer::getSingleServer();
    }
    virtual ~rpcMsg() {}

    virtual void login(const ::example::LoginInfo& loginInfo,
        cxx::function<void(int32_t ret_code, const ::example::LoginRet& loginRet)>& rsp)
    {
        int32_t roleID = loginInfo.roleid();
        int32_t roomID = loginInfo.roomid();

        // 处理请求时记录请求的来源，用于反向RPC调用，不过注意这个handle是可能失效的
        int64_t g_last_handle = _server->GetLastMessageInfo()->_remote_handle;
        int32_t status = gm->roleLogin(roleID, roomID, g_last_handle);

        std::cout << "receive rpc loginInfo: " << roleID << " + " << roomID << " = " << status << std::endl;
        ::example::LoginRet loginRet;
        loginRet.set_status(status);
        rsp(pebble::kRPC_SUCCESS, loginRet);
    }

    virtual void add(const ::example::CalRequest& request,
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

    virtual void modifyStatus(const ::example::StatusRequest& statusReq,
        cxx::function<void(int32_t ret_code, const ::example::StatusResponse& ret)>& rsp)
    {
        int32_t cmd = statusReq.cmd();
        int64_t g_last_handle = _server->GetLastMessageInfo()->_remote_handle;
        gm->modifyRoleStatus(g_last_handle, cmd);

	int32_t status = 0;

        std::cout << "receive rpc cmd: " << cmd << std::endl;
        ::example::StatusResponse ret;
        ret.set_status(status);
        rsp(pebble::kRPC_SUCCESS, ret);
    }

    virtual void move(const ::example::moveRequest& moveCMD,
        cxx::function<void(int32_t ret_code, const ::example::StatusResponse& ret)>& rsp)
    {
        int32_t dir = moveCMD.direction();
        int64_t g_last_handle = _server->GetLastMessageInfo()->_remote_handle;
        gm->inputRoleDir(g_last_handle, dir);

	int32_t status = 0;

        std::cout << "receive rpc dir: " << dir << std::endl;
        ::example::StatusResponse ret;
        ret.set_status(status);
        rsp(pebble::kRPC_SUCCESS, ret);
    } 
};
#endif

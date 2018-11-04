//
//  pebbleServer.hpp
//  testC
//
//  Created by 陈帅 on 2018/10/17.
//
#ifndef singleServer_hpp
#define singleServer_hpp

#include <string>
#include <list>
#include <map>
#include <mutex>
#include "needSaveMsg.hpp"
#include "server/pebble_server.h"
#include "server/pebble.h"
#include "rpcMsg.hpp"

using namespace std;

class singleServer
{
private:
    singleServer();
    ~singleServer();

	map<int32_t, int64_t> roleID2Handle;
	map<int64_t, int32_t> handle2RoleID;

    pebble::PebbleServer server;

    static singleServer* ss;
    list< map<string, string>* > *recQueue;
	mutex* recMutex;


    void setSendMsgTimer();
public:
	list<needSaveMsg*> *retQueue;
	mutex* retMutex;

    pebble::PebbleRpc* getBinaryRpc();
    int32_t getLastMsgRoleID();

    void bindRole2Handle(int32_t roleID);
	int64_t getHandleByRoleID(int32_t roleID);
    int32_t getRoleIDByHandle(int64_t handle);

    int32_t sendMsg(int32_t roleID, uint8_t* buff, int32_t buff_len);
    static singleServer* getSingleServer();
    int saveMsg(map<string, string>*);

    int serverStart();

    int setRecQueue(list< map<string, string>* >*);
	int setRecMutex(mutex*);

	int setRetQueue(list<needSaveMsg*>*);
	int setRetMutex(mutex*);

};

#endif

//
//  gameMgr.h
//  testC
//
//  Created by 陈帅 on 2018/9/17.
//

#ifndef gameMgr_h
#define gameMgr_h

#include <stdio.h>
#include <map>
#include <mutex>
#include <algorithm>
#include "gameMap.h"
#include "example/protobuf_rpc/net/rpcMsg.rpc.pb.h"
#include "src/framework/rpc.h"

struct needSaveMsg
{
	int64_t handle;
	const pebble::RpcHead& rpc_head;
	const uint8_t* buff;
	uint32_t buff_len;
	const pebble::OnRpcResponse& on_rsp;
	int32_t timeout_ms;
};

class gameMgr
{
private:
	list< map<string, string> >* recQueue;
	mutex* recMutex;

	list< map<string, string> >* retQueue;
	mutex* retMutex;

	static gameMgr* gm;
	map<int32_t, gameMap*> id2Map;
	gameMgr();

	map<int32_t, int64_t> role2Handle;
	map<int64_t, int32_t> handle2Role;
	map<int32_t, int32_t> roleID2MapID;

	int mapIncrValue;

	map<int, int> choosePart(vector<int> roleIDList);

	int32_t getPlayerByHandle(int64_t, player& p);
public:
	~gameMgr();
	static gameMgr* getGameMgr();

	gameMap* getMap(int32_t mapID);
	gameMap* initNewMap(vector<int> roleIDList);

	player getPlayer(int32_t roleID);

	int32_t roleLogin(int32_t roleID, int32_t mapID, int64_t handle);

	int32_t modifyRoleStatus(int64_t handle, int32_t cmd);
	int32_t inputRoleDir(int64_t handle, int32_t dir);

	map<string, string> getLegalInput(int msgID);
	
	void setRetMsg(int size, uint8_t* buff);

	int setRecQueue(list< map<string, string> >*);
	int setRecMutex(mutex*);

	int setRetQueue(list< map<string, string> >*);
	int setRetMutex(mutex*);
	void update();
};

#endif

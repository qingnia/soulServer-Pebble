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
#include "../net/needSaveMsg.hpp"

class rpcMsg;
class singleServer;
class gameMgr
{
private:
	list< map<string, string>* >* recQueue;
	mutex* recMutex;

	list<needSaveMsg*>* retQueue;
	mutex* retMutex;

	static gameMgr* gm;
	map<int32_t, gameMap*> id2Map;
	gameMgr();

	map<int32_t, int32_t> roleID2MapID;

	int mapIncrValue;

	map<int, int> choosePart(vector<int> roleIDList);

	//直接申请个流，免得每次打日志都要声明
	stringstream logStream;
public:
	singleServer* ss;
	~gameMgr();
	static gameMgr* getGameMgr();

	gameMap* getMap(int32_t mapID);
	gameMap* initNewMap(vector<int> roleIDList);

	player* getPlayer(int32_t roleID);

	retStatus roleLogin(int32_t roleID, int32_t mapID, list<playerBaseInfo>&, int32_t& roomHolder);

	list<int32_t> getBroadcastRoleIDList(int32_t roleID);

	retStatus modifyRoleStatus(int32_t roleID, int32_t cmd, int32_t& actionRoleID);
	retStatus inputRoleDir(int32_t roleID, int32_t dir, ::example::moveBroadcast&);
	retStatus attack(int32_t roleID, int32_t targetID, int32_t option, ::example::attackBroadcast&);

	map<string, string> getLegalInput(int msgID);
	
	//异步
	void setRetMsg(string function, list<int32_t> roleIDList, int size, uint8_t* buff);
	//同步
	void broadcastMsg(string function, list<int32_t> roleIDList, uint8_t* buff, int32_t buff_len);

	int setRecQueue(list< map<string, string>* >*);
	int setRecMutex(mutex*);

	int setRetQueue(list<needSaveMsg*>*);
	int setRetMutex(mutex*);
	void update();
};

#endif

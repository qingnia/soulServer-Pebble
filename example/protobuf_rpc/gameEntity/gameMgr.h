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
public:
	~gameMgr();
	static gameMgr* getGameMgr();

	gameMap* getMap(int32_t mapID);
	gameMap* initNewMap(vector<int> roleIDList);

	player getPlayer(int32_t roleID);

	int32_t roleLogin(int32_t roleID, int32_t mapID);

	list<int32_t> getBroadcastRoleIDList(int32_t roleID);

	int32_t modifyRoleStatus(int32_t roleID, int32_t cmd);
	int32_t inputRoleDir(int32_t roleID, int32_t dir);

	map<string, string> getLegalInput(int msgID);
	
	void setRetMsg(string function, list<int32_t> roleIDList, int size, uint8_t* buff);

	int setRecQueue(list< map<string, string>* >*);
	int setRecMutex(mutex*);

	int setRetQueue(list<needSaveMsg*>*);
	int setRetMutex(mutex*);
	void update();
};

#endif

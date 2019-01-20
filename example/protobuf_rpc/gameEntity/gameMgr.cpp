//
//  gameMgr.cpp
//  testC
//
//  Created by 陈帅 on 2018/9/17.
//

#include "gameMgr.h"
#include "../net/singleServer.hpp"
#include <unistd.h>

gameMgr* gameMgr::gm = new gameMgr();

gameMgr::gameMgr()
{
	this->mapIncrValue = 0;
}

gameMgr::~gameMgr()
{
}

gameMgr* gameMgr::getGameMgr()
{
	return gm;
}

int gameMgr::setRecQueue(list< map<string, string>* >* queue)
{
	this->recQueue = queue;
	return 0;
}

int gameMgr::setRecMutex(mutex* mt)
{
	this->recMutex = mt;
	return 0;
}

int gameMgr::setRetQueue(list< needSaveMsg* >* queue)
{
	this->retQueue = queue;
	return 0;
}

int gameMgr::setRetMutex(mutex* mt)
{
	this->retMutex = mt;
	return 0;
}

gameMap* gameMgr::getMap(int32_t mapID)
{
	map<int32_t, gameMap*>::iterator iter;
	iter = this->id2Map.find(mapID);
	if (iter != this->id2Map.end())
	{
		return iter->second;
	}

	//错误处理
	return nullptr;
}

player* gameMgr::getPlayer(int32_t roleID)
{
	int32_t mapID = this->roleID2MapID[roleID];
	gameMap* map = getMap(mapID);
	player* p = map->getPlayer(roleID);
stringstream ss;
ss<< "mapID:" << mapID << "roleID:" << p->getRoleID();
logInfo(ss.str());
	return p;
}

list<int32_t> gameMgr::getBroadcastRoleIDList(int32_t roleID)
{
	int32_t mapID = this->roleID2MapID[roleID];
printf("role: %d, map: %d\n", (int)roleID, (int)mapID);
	gameMap* map = this->getMap(mapID);
	return map->getRoleIDList();
}

map<int, int> gameMgr::choosePart(vector<int> roleIDList)
{
	map<int, int> roleID2Character;
	stringstream ss;
	config* conf = config::getSingleConfig();
printf("config: %d\n", (int)conf->playerConfig.size());
	list<int> leftPartList = myShuffle2List(conf->playerConfig.size());
	vector<int>::iterator roleIter;
	list<int>::iterator partIter = leftPartList.begin();
	for(roleIter = roleIDList.begin(); roleIter != roleIDList.end(); roleIter++)
	{
		string leftChara = list2String(leftPartList);
		ss<< "剩余可选人物：" << leftChara << "111111111111";
		logInfo(ss.str());
		ss.str("");
		int num = 0;
		while(partIter != leftPartList.end())
		{
			cin>>num;
			partIter = find(leftPartList.begin(), leftPartList.end(), num);
		}
		roleID2Character[*roleIter] = num;
		leftPartList.erase(partIter);
		partIter = leftPartList.end();
	}
	return roleID2Character;
}

gameMap* gameMgr::initNewMap(vector<int> roleIDList)
{
	gm->mapIncrValue++;
	map<int, int> roleID2Part = this->choosePart(roleIDList);	
	gameMap* newMap = new gameMap();
	//newMap->init(gm->mapIncrValue, roleID2Part);
	gm->id2Map.insert(pair<int, gameMap*>(gm->mapIncrValue, newMap));
	
	return nullptr;
}

retStatus gameMgr::roleLogin(int32_t roleID, int32_t mapID, list<playerBaseInfo>& baseInfos, int32_t& roomHolder)
{
	gameMap* map = getMap(mapID);
	if (map == nullptr)
	{
		map = new gameMap(mapID);
		map->init();
	}
	roleID2MapID[roleID] = mapID;
	retStatus ret = map->addNewPlayer(roleID, baseInfos, roomHolder);
cout << "gm login ret:" << ret << endl;
	this->id2Map[mapID] = map;
	return ret;
}
	
retStatus gameMgr::modifyRoleStatus(int32_t roleID, int32_t cmd, int32_t& actionRoleID)
{
	player* p = getPlayer(roleID);
	retStatus rs = p->modifyStatus(cmd, actionRoleID);
	return rs;
}
	
retStatus gameMgr::inputRoleDir(int32_t roleID, int32_t dir, ::example::moveBroadcast& sendMove)
{
	player* p = getPlayer(roleID);
	if (!p->isMyTurn())
	{
		return rsFail;
	}
	return p->move((direction)dir, sendMove);
}

retStatus gameMgr::attack(int32_t roleID, int32_t targetID, int32_t option, ::example::attackBroadcast& attackBroad)
{
	player* p = getPlayer(roleID);
	if (!p->isMyTurn())
	{
		return rsFail;
	}
	return p->attack(targetID, attackBroad);
}

map<string, string> gameMgr::getLegalInput(int msgID)
{
	std::lock_guard<std::mutex> guard(*recMutex);
	map<string, string>* legalInput;
	int tmpMsgID;
	while(true)
	{
		legalInput = recQueue->front();
		tmpMsgID = stringToNum<int>((*legalInput)["id"]);
		if(tmpMsgID == msgID)
		{
			recQueue->clear();
			break;
		}
		delete(legalInput);
	}
	return *legalInput;
}


//示例代码的发消息
/**
int __size = response.ByteSize();
uint8_t* __buff = m_server->GetBuffer(__size);
if (__buff == NULL) {
	rsp(::pebble::kPEBBLE_RPC_INSUFFICIENT_MEMORY, NULL, 0);
	return;
}
if (!response.SerializeToArray(__buff, __size)) {
	rsp(::pebble::kPEBBLE_RPC_ENCODE_BODY_FAILED, NULL, 0);
	return;
}*/
//框架只支持广播和单播，多播用单播实现，对房间内所有玩家发送
void gameMgr::setRetMsg(string function, list<int32_t> roleIDList, int size, uint8_t* buff)
{
	std::lock_guard<std::mutex> guard(*retMutex);
	needSaveMsg* nsm = new needSaveMsg(function, roleIDList, buff, size);
	//map<string, needSaveMsg*>* msg = new map<string, needSaveMsg* >(function, nsm);
	
	this->retQueue->push_back(nsm);
}

void gameMgr::broadcastMsg(string function, list<int32_t> roleIDList, uint8_t* buff, int32_t buff_len)
{
	ss->broadcastMsg(function, roleIDList, buff, buff_len);
}

void gameMgr::update()
{
	map<int, gameMap*>::iterator iter;
	for (iter = gm->id2Map.begin(); iter != gm->id2Map.end(); iter++)
	{
		gameMap* thisMap = iter->second;
		if (thisMap->getActionRoleID() > 0)
		{
			thisMap->newRun();
		}
	}
	sleep(3);

	return;
}

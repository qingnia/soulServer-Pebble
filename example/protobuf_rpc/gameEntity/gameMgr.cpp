//
//  gameMgr.cpp
//  testC
//
//  Created by 陈帅 on 2018/9/17.
//

#include "gameMgr.h"

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

int gameMgr::setInputQueue(list< map<string, string> >* queue)
{
	this->inputQueue = queue;
	return 0;
}

int gameMgr::setInputMutex(mutex* mt)
{
	this->inputMutex = mt;
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

player gameMgr::getPlayer(int32_t roleID)
{
	int32_t mapID = roleID2MapID[roleID];
	gameMap* map = getMap(mapID);
	player p = map->getPlayer(roleID);
	return p;
}

int32_t gameMgr::getPlayerByHandle(int64_t handle, player& p)
{
	map<int64_t, int32_t>::iterator iter = this->handle2Role.find(handle);
	if (iter == this->handle2Role.end())
	{
		return -1;
	}
	p = getPlayer(iter->second);
	return 0;
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
	gameMap* newMap = new gameMap(gm->mapIncrValue, roleID2Part);
	gm->id2Map.insert(pair<int, gameMap*>(gm->mapIncrValue, newMap));
	
	return nullptr;
}

int32_t gameMgr::roleLogin(int32_t roleID, int32_t mapID, int64_t handle)
{
	roleID2MapID[roleID] = mapID;
	gameMap* map = getMap(mapID);
	int ret = map->addNewPlayer(roleID);
	
	role2Handle[roleID] = handle;
	return ret;
}
	
int32_t gameMgr::modifyRoleStatus(int64_t handle, int32_t cmd)
{
	player p = player();
	int32_t ret = getPlayerByHandle(handle, p);
	p.modifyStatus(cmd);
	return ret;
}
	
int32_t gameMgr::inputRoleDir(int64_t handle, int32_t dir)
{
	player p = player();
	int32_t ret = getPlayerByHandle(handle, p);
	p.modifyStatus(dir);
	return ret;
}

map<string, string> gameMgr::getLegalInput(int msgID)
{
	std::lock_guard<std::mutex> guard(*inputMutex);
	map<string, string> legalInput;
	int tmpMsgID;
	while(true)
	{
		legalInput = inputQueue->front();
		tmpMsgID = stringToNum<int>(legalInput["id"]);
		if(tmpMsgID == msgID)
		{
			inputQueue->clear();
			break;
		}
	}
	return legalInput;
}

void gameMgr::update()
{
	map<int, gameMap*>::iterator iter;
	for (iter = gm->id2Map.begin(); iter != gm->id2Map.end(); iter++)
	{
		gameMap* thisMap = iter->second;
		thisMap->run();
	}

	return;
}

//
//  main.cpp
//  testC
//
//  Created by 陈帅 on 2018/9/10.
//

#include "gameMap.h"

gameMap::gameMap()
{
    this->actionRoleID = 0;
}

gameMap::~gameMap()
{
}

int32_t gameMap::getActionRoleID()
{
    return this->actionRoleID;
}

retStatus gameMap::addNewPlayer(int32_t roleID, list<playerBaseInfo>& baseInfos)
{
	if (this->actionRoleID > 0)
	{
		return rsFail;
	}
    player p(roleID, this->m_id);
    this->playerList.push_back(p);

    stringstream ss;
    ss<<"新玩家进入 roleid:" << roleID << "ret:" << rsSuccess;
    logInfo(ss.str());

	list<player>::iterator iter;
	for(iter = this->playerList.begin(); iter != this->playerList.end(); iter++)
	{
		playerBaseInfo p;
		p.roleID = iter->getRoleID();
		p.name = iter->getName();
		p.ps = iter->getStatus();
		baseInfos.push_back(p);
	}
    return rsSuccess;
}

int gameMap::initPlayerList(map<int, int> roleID2PartID)
{
    stringstream ss;
    ss<<"新地图生成，玩家一共"<< roleID2PartID.size() << "人";
    logInfo(ss.str());

    config* conf = config::getSingleConfig();
    map<int, int>::iterator iter;
    map<string, string> playerConfig;
    for(iter = roleID2PartID.begin(); iter != roleID2PartID.end(); iter++)
    {
        playerConfig = conf->playerConfig[iter->second];
        player p(iter->first, this->m_id, playerConfig);
        this->playerList.push_back(p);
    }
    return 0;
}

int gameMap::initCardList()
{
	map<int, map<string, string> >::iterator confIter;
//    card* newCard;
	config* conf = config::getSingleConfig();
    
    //房间初始化
    this->roomList = myShuffle2List(conf->roomConfig.size());
    for(confIter = conf->roomConfig.begin(); confIter != conf->roomConfig.end(); confIter++)
    {
        this->id2room[confIter->first] = (card*) new roomCard(confIter->second);
    }
    this->roomIter = this->roomList.begin();

	//事件初始化
	this->issueList = myShuffle2List(conf->issueConfig.size());
	for (confIter = conf->issueConfig.begin(); confIter != conf->issueConfig.end(); confIter++)
	{
		
		this->id2issue[confIter->first] = (card*) new issueCard(confIter->second);
	}
	this->issueIter = this->issueList.begin();
    
    //物品初始化
    this->resList = myShuffle2List(conf->resConfig.size());
	map<string, string> oneConfig;
    for(confIter = conf->resConfig.begin(); confIter != conf->resConfig.end(); confIter++)
    {
		oneConfig = confIter->second;
		if (oneConfig["cardType"] == "-1")
		{
			this->id2info[confIter->first] = (card*) new resCard(confIter->second);
		}
		else
		{
			this->id2res[confIter->first] = (card*) new resCard(confIter->second);
		}
    }
    this->resIter = this->resList.begin();
	this->infoIter = this->infoList.begin();
    
	/**
    this->infoList = myShuffle2List(conf->infoConfig.size());
	for (confIter = conf->infoConfig.begin(); confIter != conf->infoConfig.end(); confIter++)
	{
		
	} */
    
    return 0;
}

int gameMap::initActionList()
{
    action act = action(atStart);
    this->actionList.push_back(act);
	list<player>::iterator iter;
	for (iter = playerList.begin();
		iter != playerList.end(); iter++)
	{
        //actionType at = move;
        action startAct(atMove, &(*iter));
        this->actionList.push_back(startAct);

        action stopAct(atStop, &(*iter));
        this->actionList.push_back(stopAct);
	}

    action overAct(atOver);
    this->actionList.push_back(overAct);

    this->nextAction = this->actionList.begin();
    return 0;
}

bool gameMap::init(int mapID, map<int, int> roleID2PartID)
{
    if(this->pos2room[50][50])
    {
		return true;
    }

    if (roleID2PartID.size() <= 0)
    {
        //错误处理
    }
    //x,y是坐标,楼梯位置是（50，50）
    //为了保证可用，地图大小100*100
    //大厅看作连通的两个房间加上楼梯

    this->m_height = 1;
    this->m_length = 100;
    this->m_width = 100;
	this->m_id = mapID;
    this->m_infoNum = 0;
    this->m_process = 0;

	/**
    int** ret = (int**)malloc(this->m_length * sizeof(int));
    for (int i = 0; i < this->m_length; i++) {
        for (int j = 0; j < this->m_width; j++) {
            ret[i] = (int*)malloc(this->m_width * sizeof(int));
        }
    }
	
	for (int i = 0; i < this->m_length; i++)
	{
		this->pos2room[i] = map<int, int> newMap(50);
	}
	*/
	this->pos2room[50][50] = 1;
	this->pos2room[51][50] = 2;
	this->pos2room[52][50] = 3;
    //ret[50][50] = 1;
    //ret[51][50] = 2;
    //ret[52][50] = 3;
    //this->pos2room = ret;
    //假定两人玩游戏
    this->initPlayerList(roleID2PartID);
    this->initCardList();
	this->initActionList();
	return true;
}

roomCard* gameMap::getRoomByID(int roomID)
{
    map<int, card*>::iterator iter;
    iter = this->id2room.find(roomID);
    if (iter != this->id2room.end())
    {
        return dynamic_cast<roomCard*>(iter->second);
    }
    return nullptr;
}

roomCard* gameMap::getRoom(position pos)
{
    int x = pos.x;
    int y = pos.y;
    int roomID = this->pos2room[x][y];

    return this->getRoomByID(roomID);
    //roomCard* newRoom = new roomCard(roomID);
    //this->id2room.insert(pair<int, roomCard*>(roomID, newRoom));
    //return nullptr;
}

list<int32_t> gameMap::getRoleIDList()
{
    list<int32_t> l;
    list<player>::iterator iter;
    for(iter = playerList.begin(); iter != playerList.end(); iter++)
    {
        l.push_back(iter->getRoleID());
    }
    return l;
}

player gameMap::getPlayer(int id)
{
    list<player>::iterator iter;
    for(iter = this->playerList.begin(); iter != this->playerList.end(); iter++)
    {
        if ((*iter).getID() == id)
        {
            return *iter;
        }
    }
    //错误处理
    //todo
	return player();
}

roomCard* gameMap::bindNewRoom(int floor, position pos)
{
    roomCard* newRoom;
    while(true)
    {
		int roomID = this->roomList.front();
		this->roomList.pop_front();
		newRoom = this->getRoomByID(roomID);
		if (in_vector(floor, newRoom->suiteLayer))
		{
			this->pos2room[pos.x][pos.y] = roomID;
			break;
		}
		this->roomList.push_back(roomID);
    }
    return newRoom;
}

issueCard* gameMap::getNewIssue()
{
    issueCard* newIssue;
	int issueID = this->issueList.front();
	this->issueList.pop_front();
    
    config* conf = config::getSingleConfig();
    map<string, string> issueConfig = conf->getConfig(ctIssue, issueID);
    newIssue = new issueCard(issueConfig);
    return newIssue;
}

resCard* gameMap::getNewInfo()
{
    resCard* newInfo;
	int infoID = this->infoList.front();
	this->infoList.pop_front();

    this->m_infoNum++;

    config* conf = config::getSingleConfig();
    map<string, string> infoConfig = conf->getConfig(ctInfo, infoID);
    newInfo = new resCard(infoConfig);
    return newInfo;
}

resCard* gameMap::getNewRes()
{
	resCard* newRes;
	int infoID = this->resList.front();
	this->resList.pop_front();

	config* conf = config::getSingleConfig();
	map<string, string> resConfig = conf->getConfig(ctInfo, infoID);
	newRes = new resCard(resConfig);
	return newRes;
}


//用action作回调，实际效果不好，还不如轮流查询。
//中间状态作用于其他人身上，会作为补充询问在中间某个流程内发起，不影响整体流程
//如万箭齐发，作为格外流程处理
int gameMap::run()
{
	stringstream ss;
    //放弃回调式处理，简单分为：开始、移动、结束，每个阶段循环处理每个玩家
    list<player>::iterator iter;
    //开始阶段
    ss << "新一轮开始，开始阶段："; 
	logInfo(ss.str());
    ss.clear();
    for(iter = this->playerList.begin(); iter != this->playerList.end(); iter++)
    {
        iter->start();
    }
    for(iter = this->playerList.begin(); iter != this->playerList.end(); iter++)
    {
        iter->move();
    }
	ss << "本轮结束";
	logInfo(ss.str());
    for(iter = this->playerList.begin(); iter != this->playerList.end(); iter++)
    {
        iter->stop();
    }
    return 0;
}

//这游戏更接近棋牌，系统控制当前行动玩家，然后响应玩家输入即可
void gameMap::newRun()
{
	//定时器只需要检查玩家状态就行
	//当所有玩家都标记已行动，就开始本轮结算，然后开始新一轮
	stringstream ss;
	list<player>::iterator iter;
	for (iter = this->playerList.begin(); iter != this->playerList.end(); iter++)
	{
		if (!iter->isActionDone())
		{
			this->actionRoleID = iter->getID();
			break;
		}
	}
	if (iter == this->playerList.end())
	{
		this->actionRoleID = this->playerList.begin()->getRoleID();
		//开始阶段
		ss << "新一轮开始";
		logInfo(ss.str());
	}
}

position gameMap::inputPosition()
{
    int x, y;
    cin>>x;
    cin>>y;
    return position(x, y);
}

int gameMap::getInfoNum()
{
    return this->m_infoNum;
}

int gameMap::getProcess()
{
    return this->m_process;
}

int gameMap::incrProcess()
{
    this->m_process++;
    return this->m_process;
}

bool gameMap::unravelRiddle(position pos, int playerID)
{
    this->m_process++;
    //roomCard* room = this->getRoom(pos);

    return false;
}

bool gameMap::tryEnd()
{
    return false;
}

list<int> gameMap::getCanAttackRoleIDList(player* p)
{
	list<int> roleIDList;
	list<player>::iterator iter;
	for (iter = this->playerList.begin(); iter != this->playerList.end(); iter++)
	{
		if (iter->getRoleID() == p->getRoleID())
		{
			continue;
		}
		//目前只考虑没武器的情况，即必须有玩家在相同房间才可以攻击
		if (iter->getMyRoom() == p->getMyRoom())
		{
			roleIDList.push_back(iter->getRoleID());
		}
	}
	return roleIDList;
}

retStatus gameMap::tryStart()
{
	logStream << "尝试开始游戏";
	logInfo(logStream.str());
	logStream.clear();
	list<player>::iterator iter;
	for (iter = this->playerList.begin(); iter != this->playerList.end(); iter++)
	{
		if (iter->getStatus() != psReady)
		{
			return rsFail;
		}
	}

	int first = random(this->playerList.size());
	int index = 0;
	for (iter = this->playerList.begin(); iter != this->playerList.end(); iter++)
	{
		if (index < first)
		{
			iter->actionDone = true;
		}
		else if (index == first)
		{
			iter->actionDone = false;
			this->actionRoleID = iter->getRoleID();
		}
		else
		{
			iter->actionDone = false;
		}
		iter->modifyStatus(psStart);
		index++;
		logStream << "游戏开始了";
		logInfo(logStream.str());
		logStream.clear();
	}
	return rsSuccess;
}

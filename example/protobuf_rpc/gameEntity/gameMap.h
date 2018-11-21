//
//  main.hpp
//  testC
//
//  Created by 陈帅 on 2018/9/16.
//

#ifndef gameMap_h
#define gameMap_h

#include <stdio.h>
#include <map>
#include <vector>
#include "position.h"
#include "roomCard.h"
#include "issueCard.h"
#include "resCard.h"
#include "player.hpp"
#include "action.h"
#include "../utils/commonFun.h"
#include "example/protobuf_rpc/net/rpcMsg.rpc.pb.h"

struct playerBaseInfo
{
	int32_t roleID;
	string name;
	playerStatus ps;
};

class gameMap{
    int m_id, m_width, m_length, m_height, m_infoNum, m_process;
    int32_t actionRoleID, roomHolder;
    
    list<action>::iterator nextAction;

    //int** pos2room;
	map<int, map<int, int> > pos2room;

    map<int, card* > id2room, id2issue, id2res, id2info;

    list<player*> playerList;
    list<action> actionList;

    list<int>::iterator roomIter, resIter, issueIter, infoIter;
    list<int>           roomList, resList, issueList, infoList;


    int initPlayerList(map<int, int>);
    int initActionList();
    int initCardList();

    //等待输入函数
    position inputPosition();

public:
    int32_t getActionRoleID(), getRoomHolder();
	retStatus tryStart();

	gameMap();
	gameMap(int32_t);
    bool init();

    retStatus addNewPlayer(int32_t roleID, list<playerBaseInfo>&, int32_t& roomHolder);
    list<int32_t> getRoleIDList();

    roomCard* getRoom(position pos);
    roomCard* getRoomByID(int roomID);
    player* getPlayer(int32_t id);

    roomCard* bindNewRoom(int floor, position pos);
	resCard* getNewRes();
	resCard* getNewInfo();
    issueCard* getNewIssue();

	list<int> getCanAttackRoleIDList(player*);
    int run();
    void newRun();

    bool unravelRiddle(position, int playerID);
    bool tryEnd();

    int getInfoNum();
    int getProcess();
    int incrProcess();
    
	~gameMap();
};

#endif /* gameMap_h */

//
//  player.hpp
//  testC
//
//  Created by 陈帅 on 2018/9/11.
//

#ifndef player_hpp
#define player_hpp

#include <stdio.h>
#include <list>
#include <iostream>
#include "position.h"

#include "../utils/diyType.h"
#include "example/protobuf_rpc//net/rpcMsg.pb.h"
#include "resCard.h"
#include "roomCard.h"

class gameMap;
class gameMgr;
using namespace std;
using namespace example;
class player {
    playerStatus m_ps;
    position pos;

    string m_name;
    int age, moveNum, m_floor;
    int m_id;
	int32_t m_roleID, m_mapID;
    
    map<examType, int> et2level;
    map<examType, vector<int> > etLevel2value;
    list<resCard*> resList;
    list<resCard*> infoList;
    list<int> skills;

	direction inputDir();
	list<int> rollDice(examType, int forceDiceNum = 0);
	template<class Type> Type inputFromList(const list<Type>&);
    int useWeapon();
    int moveTo(direction, ::example::moveBroadcast&);

    int gainNewItem(configType);

    int getETValue(examType);

    bool canAttack(int32_t targetID);
public:
    int incrETLevel(examType, int);

    player();
    player(int32_t roleID, int32_t mapID);
    retStatus init(int32_t characterID);

    gameMap* getMyMap();

    int getID();
	int32_t getRoleID(), getMapID();
	string getName();
    roomCard* getMyRoom();
	bool actionDone;

//    oper getOperate();
    bool isMyTurn();
    int start();
	retStatus move(direction, ::example::moveBroadcast&);
    int stop(::example::moveBroadcast& mb);

    bool isActionDone();

    retStatus attack(int32_t targetID, ::example::attackBroadcast& attackBroad);
    int useSkill(int id);

	int enterRoom(roomCard*, bool isNewRoom, ::example::moveBroadcast& mb);
	bool leaveRoom(roomCard*);   //从上一回合进入，这一回合离开，是“离开房间”
    bool passRoom(roomCard*);    //这一回合进入，这一回合离开，是“通过房间”

    int excuteExam(examine);
    int excutePunish(examType, int);

    bool gainBuff(cardUseType, card*);

    bool getReality();

    int changeNewRoomRotation(direction, roomCard*);

    ////////////组队房///////////////
	playerStatus getStatus();
    retStatus modifyStatus(int32_t, int32_t&);
};

#endif /* player_hpp */

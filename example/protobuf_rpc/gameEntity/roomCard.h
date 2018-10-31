//
//  main.hpp
//  testC
//
//  Created by 陈帅 on 2018/9/16.
//

#ifndef roomCard_h
#define roomCard_h

#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "card.h"
#include "../utils/commonFun.h"
#include "../utils/diyType.h"
#include "../utils/config.h"

using namespace std;

class roomCard: public card
{
    moveRoomType mrt;
    int roomRotation;

    roomCard();
public:
    virtual ~roomCard();
    
    //bool init(map<string, string> roomCardConfig);
    roomCard(map<string, string>);

    vector<int> suiteLayer;

    //int left, right, up, down;
	vector<int> exportArray;

	moveRoomType testType;

	configType type;
	bool canPass(direction);

    bool needExam(moveRoomType);

    bool canChangeRotation();
    bool changeRotation(direction fromDir, direction choseDir);

};

#endif /* roomCard_hpp */
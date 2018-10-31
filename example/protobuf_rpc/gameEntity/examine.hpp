//
//  main.hpp
//  testC
//
//  Created by 陈帅 on 2018/9/16.
//

#ifndef examine_h
#define examine_h

#include <stdio.h>
#include <list>
#include <numeric>
#include <iostream>
#include "../utils/commonFun.h"
#include "../utils/config.h"
//#include "player.hpp"
using namespace std;

struct effect{
    //影响类型
    examType et;
    int min, max, eNum;

    effect(string str)
    {
        vector<int> conf = split<int>(str, "_");
        this->et = (examType)conf[0];
        this->min = conf[1];
        this->max = conf[2];
        this->eNum = conf[3];
    }

    effect()
    {
        this->et = etNone;
        this->min = 0;
        this->max = 0;
        this->eNum = 0;
    }
};

class examine{

private:

public:
    examine();
    examine(int);

    string name, enlishName;
    examType et;

    void showMsg();
    
    int attackValue;
    effect attackEffect;

    //区间判定
    //（1，3，6）氛围0-1，2-3，4-6三个区间
    list<effect> efList;

 //   void affect(player p);

    //bool excutePunish(player, effect);
};

#endif /* examine_hpp */

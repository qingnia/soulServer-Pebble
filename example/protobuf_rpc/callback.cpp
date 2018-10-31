//
//  main.cpp
//  testC
//
//  Created by 陈帅 on 2018/9/10.
//
#include "callback.h"


void move(player p)
{
    p.move();
}

void stop(player p, position)
{
    p.stop();
}

void action(player p, position pos, FuncCallBack f)
{
    f(p, pos);
}
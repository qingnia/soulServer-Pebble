//
//  main.cpp
//  testC
//
//  Created by 陈帅 on 2018/9/10.
//

#include "action.h"

action::action()
{
    ;
}

action::action(actionType at)
{
    this->at = at;
}

action::action(actionType at, player* p)
{
    this->at = at;
	this->p = p;
}

action::action(actionType at, player* p, position pos)
{
    this->at = at;
	this->p = p;
    this->pos = pos;
}

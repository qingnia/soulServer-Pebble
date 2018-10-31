//
//  main.hpp
//  testC
//
//  Created by 陈帅 on 2018/9/16.
//

#ifndef action_h
#define action_h

#include <stdio.h>
#include "position.h"
#include "player.hpp"

class action{

public:
    action();
    action(actionType at);
    action(actionType at, player* p);
    action(actionType at, player* p, position pos);

	player* p;
    position pos;
    actionType at;
};

#endif /* action_hpp */
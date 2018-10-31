//
//  main.hpp
//  testC
//
//  Created by 陈帅 on 2018/9/16.
//

#ifndef position_h
#define position_h

#include "../utils/diyType.h"
#include <stdio.h>

class position{

public:
    position();
    position(int x, int y);
    int x;
    int y;

	position* getNeibourPos(direction);
    int setPos(int x, int y);
};
#endif /* position_hpp */
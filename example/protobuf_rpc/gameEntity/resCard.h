//
//  resCard.h
//  testC
//
//  Created by 陈帅 on 2018/9/21.
//

#ifndef resCard_h
#define resCard_h

#include <stdio.h>
#include "card.h"
#include "../utils/config.h"

class resCard: public card
{
private:
	int m_cct;
	cardUseType useType;
	resCard();

public:
	resCard(map<string, string>);
	bool isWeapon();
	virtual ~resCard();

};

#endif
//
//  issueCard.h
//  testC
//
//  Created by 陈帅 on 2018/9/22.
//
#ifndef issueCard_h
#define issueCard_h

#include "card.h"
#include<map>

using namespace std;
class issueCard: public card
{
	int record;
	issueCard();
public:
	issueCard(map<string, string>);
	virtual ~issueCard();

};

#endif
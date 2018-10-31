//
//  card.h
//  testC
//
//  Created by 陈帅 on 2018/9/22.
//

#ifndef Card_h
#define Card_h

#include <stdio.h>
#include <string>
#include "examine.hpp"

using namespace std;

class card
{
private:
protected:
	int m_id;
	string m_name, m_enlishName, m_preDesc, m_desc;
	map<examType, int> buff;	//拿到物品时
	map<examType, int> deBuff;	//丢失物品时

	bool showMsg();
public:
	examine cardExam;

	string getName();
	string getEnlishName();
	string getDesc();
	int getID();

	map<examType, int> getBuff(cardUseType);
	
	card();
	virtual ~card();


};

#endif
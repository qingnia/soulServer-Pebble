//
//  examine.cpp
//  testC
//
//  Created by 陈帅 on 2018/9/10.
//
#include "examine.hpp"

examine::examine()
{
    this->et = etNone;
}

examine::examine(int examID)
{
	config* conf = config::getSingleConfig();
    map<string, string> examConfig = conf->getConfig(ctExam, examID);
    //房间初始化
    map<string, string>::iterator iter;
 	string key, value;

	for (iter = examConfig.begin(); iter != examConfig.end(); iter++)
	{
		key = iter->first;
		value = iter->second;
		if (key == "name")
		{
			this->name = value;
		}
		else if(key == "enlishName")
		{
			this->enlishName = value;
		}
        else if(key == "examType")
        {
            this->et = (examType)stringToNum<int>(value);
        }
        else if (key == "attack")
        {
            this->attackValue = stringToNum<int>(value);
        }
        else if (key == "attackEffect")
        {
            //攻击的影响，默认配置必须是：0_0_0_0
            //如果有影响，这样：1_0_0_3
            this->attackEffect = effect(value);
        }
        else if(key == "effect")
        {
            //要求配置比较复杂：et1_min1_max1_eNum1|et2_min2_max2_eNum2|....
            vector<string> effectConfig = split<string>(value, "|");
            vector<string>::iterator iter;
            for(iter = effectConfig.begin(); iter != effectConfig.end(); iter++)
            {
                this->efList.push_back(effect(*iter));
            }
        }
	}
}

void examine::showMsg()
{
	stringstream ss;
	string efAttr = getETString(this->et);
	ss << "你必须以“" << efAttr << "”进行能力考验\n";
    //printf("你必须以 %d 进行能力考验\n", (int)(this->et));
    list<effect>::iterator iter;
    for(iter = this->efList.begin(); iter != this->efList.end(); iter++)
    {
		efAttr = getETString(this->et);
		ss << "如果得分在" << iter->min << "和" << iter->max << "之间则" << efAttr;
		if (iter->eNum > 0)
		{
			ss << "增加" << iter->eNum << "个等级\n";
		}
		else
		{
			ss << "受到" << iter->eNum << "点损伤\n";
		}
    }
    //string roomID(tmp);
    //string roomID = new string(tmp)
    //string info = "你必须以%d进行能力考验";
	logInfo(ss.str());
    //cout<<""<<endl;
}
/**
void examine::affect(player p)
{
    if (this->et == etNone)
    {
        return;
    }
    this->showMsg();
    list<int> diceNums = p.rollDice(this->et);
    int score = accumulate(diceNums.begin(), diceNums.end(), 0);
    if(this->attackValue > 0)
    {
        int compareNum = random(2 * this->attackValue);
        if(score < compareNum)
        {
            this->excutePunish(p, this->attackEffect);
        }
    }
    else
    {
        list<effect>::iterator iter;
        for (iter = this->efList.begin(); iter != this->efList.end(); iter++)
	    {
           effect ef = *iter;
           if (score >= ef.min & score <= ef.max)
           {
               this->excutePunish(p, ef);
               break;
           }
	    }
    }
}

bool examine::excutePunish(player p, effect ef)
{
    examType attribute = ef.et;
    if (ef.et == etPhysicalDamage)
    {
        string tip = "你收到%d点物理损伤，请选择1:速度，2:力量";
        cout<<tip<<endl;
        //选择
        attribute = etSpeed;
    }
    else if (ef.et == etMindDamage)
    {
        //选择
        attribute = etSpirit;
    }
    switch(attribute)
    {
    case etSpeed:
        p.incrSpeed(ef.eNum);
        break;
    case etStrength:
        p.incrStrength(ef.eNum);
        break;
    case etSpirit:
        p.incrSpirit(ef.eNum);
        break;
    case etKnowledge:
        p.incrKnowledge(ef.eNum);
        break;
    default:
        break;
    }
    return true;
} 
*/
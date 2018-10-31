//
//  main.cpp
//  testC
//
//  Created by 陈帅 on 2018/9/10.
//

#include "roomCard.h"
//#include <codecvt>

roomCard::roomCard()
{

}

roomCard::~roomCard()
{
}

roomCard::roomCard(map<string, string> roomConfig)
{
	//config* conf = config::getSingleConfig();
	//map<string, string> roomConfig = conf->getConfig(roomCard, id);

	map<string, string>::iterator iter;
	string key, value;
	for (iter = roomConfig.begin(); iter != roomConfig.end(); iter++)
	{
		key = iter->first;
		value = iter->second;
		if (key == "name")
		{
			this->m_name = value;
		}
		else if(key == "enlishName")
		{
			this->m_enlishName = value;
		}
		else if (key == "desc")
		{
			this->m_desc = value;
		}
		else if (key == "layer")
		{
			this->suiteLayer = split<int>(value, "|");
		}
		else if (key == "canPassArray")
		{
			this->exportArray = split<int>(value, "|");
		}
		else if (key == "type")
		{
			this->type = configType(stringToNum<int>(value));
		}
		else if (key == "mrtType")
		{
			this->mrt = moveRoomType(stringToNum<int>(value));
		}
		else if (key == "examID")
		{
			this->cardExam = examine(stringToNum<int>(value));
		}
	}
	//map<string, string>::iterator iter = myFind(roomConfig.begin(), roomConfig.end(), "name");
    //stringstream ss;
    //ss<<id;
    //string roomID = ss.str();
    
    //char tmp[32];
    //sprintf(tmp, "%d", id);
    //string roomID(tmp);
    //string roomID = new string(tmp); 
    //readCsvData("room.csv");
	//return true;
}

bool roomCard::canPass(direction dir)
{
	int checkDir = (int)dir;
	if (this->exportArray[checkDir] == 1)
	{
		return true;
	}
	return false;
	/**
	switch (dir)
	{
	case dirUp:
		if (this->up == 1)
		{
			return true;
		}
		break;
	case dirDown:
		if (this->down == 1)
		{
			return true;
		}
		break;
	case dirLeft:
		if (this->left == 1)
		{
			return true;
		}
		break;
	case dirRight:
		if (this->right == 1)
			return true;
		break;
	default:
		return false;
		break;
	}
	*/
	//return false;
}

bool roomCard::canChangeRotation()
{
	stringstream ss;
    int exportNum = accumulate(this->exportArray.begin(),this->exportArray.end(),0);
	if (exportNum > 1 && exportNum < 4)
	{
		ss<<"新房间出口不止一个，并且不是四通，请选择需要哪个门面向原房间。开着的门分别为：";
		for(int i = 0; i < 4; i++)
		{
			if (this->exportArray[i] == 1)
			{
				string dirString = getDirString((direction)i);
				ss<<dirString<<",   ";
			}
		}
		return true;
	}
	return false;
}

bool roomCard::changeRotation(direction fromDir, direction choseDir)
{
	if (choseDir == dirStop)
	{
		return false;
	}
	
	int exportNum = accumulate(this->exportArray.begin(),this->exportArray.end(),0);
	if (exportNum == 4)
	{
		return true;
	}

	if (exportNum == 1)
	{
		for(int i = 0; i < 4; i++)
		{
			if (this->exportArray[i] == 1)
			{
				choseDir = (direction)i;
				break;
			}
		}	
	}
	else if (this->exportArray[choseDir] != 1)
	{
		return false;
	}

	direction faceDir = reverseDir(fromDir);
	if (faceDir == choseDir)
	{
		return true;
	}

	int diff = faceDir - choseDir;
	if (diff < 0)
	{
		diff = 4 + diff;
	}
	vector<int> tmp = this->exportArray;
	for(int i = 0; i < 4; i++)
	{
		int index = i + diff;
		if (index > 3)
		{
			index = index - 4;
		}
		this->exportArray[index] = tmp[i];
	}
	return true;
}

bool roomCard::needExam(moveRoomType mrt)
{
	if (mrt == this->mrt)
	{
		return true;
	}
	return false;
}

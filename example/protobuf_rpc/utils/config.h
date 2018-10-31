//
//  config.h
//  testC
//
//  Created by 陈帅 on 2018/9/19.
//

#ifndef config_h
#define config_h

#include <stdio.h>
#include "commonFun.h"
#include "diyType.h"

enum configReadType
{
	numberInt,
	numberFloat,
	stringDesc,
	vectorInt,
	mapKey,
	mapValue,
};

struct fileConfig
{
	string fname;
	map<string, configReadType> kvMap;		//完全依赖表里面的字段名
};

class config
{
private:
	config();
	static config* gameConfig;

	

public:
	static config* getSingleConfig();
	~config();

	map<int, map<string, string> > roomConfig, resConfig, infoConfig, issueConfig, examConfig, playerConfig;

	map<string, string> getConfig(configType, int);
};

#endif
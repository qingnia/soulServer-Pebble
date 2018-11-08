//
//  config.cpp
//  testC
//
//  Created by 陈帅 on 2018/9/19.
//

#include "config.h"

config* config::gameConfig = new config();
config::config()
{
	//this->roomConfig = readCsvData("example/protobuf_rpc/tables/Room.csv");
	//this->issueConfig = readCsvData("example/protobuf_rpc/tables/Issue.csv");
	//this->resConfig = readCsvData("example/protobuf_rpc/tables/Res.csv");
	//this->infoConfig = readCsvData("example/protobuf_rpc/tables/Info.csv");
	//this->examConfig = readCsvData("example/protobuf_rpc/tables/Examine.csv");
	//this->examConfig = readCsvData("example/protobuf_rpc/tables/Player.csv");
	//this->playerConfig = readCsvData("../tables/Player.csv");
	this->roomConfig = readCsvData("/home/qingniao/Pebble/example/protobuf_rpc/tables/Room.csv");
	this->issueConfig = readCsvData("/home/qingniao/Pebble/example/protobuf_rpc/tables/Issue.csv");
	this->resConfig = readCsvData("/home/qingniao/Pebble/example/protobuf_rpc/tables/Res.csv");
	this->infoConfig = readCsvData("/home/qingniao/Pebble/example/protobuf_rpc/tables/Info.csv");
	this->examConfig = readCsvData("/home/qingniao/Pebble/example/protobuf_rpc/tables/Examine.csv");
	this->playerConfig = readCsvData("/home/qingniao/Pebble/example/protobuf_rpc/tables/Player.csv");
}

config::~config()
{
}

config* config::getSingleConfig()
{
	if (gameConfig == nullptr)
	{
		gameConfig = new config();
	}
	return gameConfig;
}

map<string, string> config::getConfig(configType type, int id)
{
	//map<string, string> fc;
	map<int, map<string, string> >::iterator iter;
	switch (type)
	{
	case ctRoom:
		if (this->roomConfig.size() <= 0)
		{
			this->roomConfig = readCsvData("../tables/Room.csv");
		}
		iter = this->roomConfig.find(id);
		if (iter != this->roomConfig.end())
		{
			return iter->second;
		}
		//fc.fname = "../config/Room.csv";
		//fc.kvMap[string("Name")] = stringDesc;
		//fc.kvMap[string("enlishName")] = stringDesc;
		//fc.kvMap[string("layer")] = vectorInt;
		//fc.kvMap[string("type")] = numberInt;
		//this->roomConfig = loadConfig(fc);
		break;
	case ctIssue:
		if (this->issueConfig.size() <= 0)
		{
			this->issueConfig = readCsvData("../tables/Issue.csv");
		}
		iter = this->issueConfig.find(id);
		if (iter != this->issueConfig.end())
		{
			return iter->second;
		}
		break;
	case ctRes:
		iter = this->resConfig.find(id);
		if (iter != this->resConfig.end())
		{
			return iter->second;
		}
		break;
	case ctInfo:
		iter = this->infoConfig.find(id);
		if (iter != this->infoConfig.end())
		{
			return iter->second;
		}
		break;
	case ctExam:
		if (this->examConfig.size() <= 0)
		{
			this->examConfig = readCsvData("../tables/Exam.csv");
		}
		iter = this->examConfig.find(id);
		if (iter != this->examConfig.end())
		{
			return iter->second;
		}
		break;
	default:
		break;
	}
	return iter->second;
}

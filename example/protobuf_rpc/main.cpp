//  main.h
//  testC
//
//  Created by 陈帅 on 2018/9/21.
//

// NewTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>
#include <thread>
#include <mutex>
#include "gameEntity/gameMgr.h"
#include "net/singleServer.hpp"

std::list< map<string, string>* > recList;
std::list<needSaveMsg*> retList;
std::mutex recMutex;
std::mutex retMutex;

void mainGame(gameMgr* gm)
{
        vector<int> roleIDList = {1001, 1002, 1003};
        gm->setRecQueue(&recList);
        gm->setRecMutex(&recMutex);
		gm->setRetQueue(&retList);
		gm->setRetMutex(&retMutex);
        gm->initNewMap(roleIDList);
        /*
        while (true)
        {
                std::lock_guard<std::mutex> guard(myMutex);
                for(auto &iter: myList)
                        std::cout<<iter<<",";

                gm->update();
        }*/
}

void serverSocket(singleServer* ss, int max, int interval)
{
        ss->setRecQueue(&recList);
        ss->setRecMutex(&recMutex);
		ss->setRetQueue(&retList);
		ss->setRetMutex(&retMutex);
        ss->serverStart();
printf("4444444\n");

/*
        std::lock_guard<std::mutex> guard(myMutex);
        for(int i=0;i<max;++i) {
                if(i%interval==0)
                        myList.push_back(i);
        }*/


}

int main()
{
	gameMgr* gm = gameMgr::getGameMgr();
	singleServer* ss = singleServer::getSingleServer();
	gm->ss = ss;

	int max = 15;
	std::thread t1(serverSocket, ss, max, 1);
	std::thread t2(mainGame, gm);
	t1.join();
	t2.join();

        return 0;

}

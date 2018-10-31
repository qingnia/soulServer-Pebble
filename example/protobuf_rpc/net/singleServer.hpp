//
//  pebbleServer.hpp
//  testC
//
//  Created by 陈帅 on 2018/10/17.
//
#ifndef singleServer_hpp
#define singleServer_hpp

#include <string>
#include <list>
#include <map>
#include <mutex>

using namespace std;

class singleServer
{
private:
    singleServer();
    ~singleServer();

    static singleServer* ss;
    list< map<string, string> > *inputQueue;
	mutex* inputMutex;

public:
    static singleServer* getSingleServer();
    int saveMsg(map<string, string>);

    int serverStart();

    int setInputQueue(list< map<string, string> >*);
	int setInputMutex(mutex*);

};

#endif

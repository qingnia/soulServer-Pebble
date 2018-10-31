//
//  commonFun.h
//  testC
//
//  Created by 陈帅 on 2018/9/19.
//

#ifndef commonFun_h
#define commonFun_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <list>
#include <map>
#include <fstream>
#include <sstream>
#include <random>
#include <chrono>
#include <algorithm>
#include <locale>
#include <codecvt.h>
#include <locale.h>
#include "diyType.h"
#include "log.hpp"

using namespace std;

#define random(x) (rand()%x)

vector<int> myShuffle(int num);
list<int> myShuffle2List(int num);

direction reverseDir(direction);

bool in_vector(int value, vector<int> vec);

//template <class Type>
//Type stringToNum(const string str);

template <class Type>
Type stringToNum(const string& str)
{
	stringstream iss(str);
	Type num;
	iss >> num;
	return num;
}

// any simple type convert to string
template<class src_type>
string type2str(src_type src)
{
	stringstream ss;
	ss << src;
	string ret;
	ss >> ret;

	return ret;
}

template <class Iterator, class T>
Iterator myFind(Iterator begin, Iterator end, const T &value);

//template <class Type>
//vector<Type> split(const string &str, const string &pattern);

template <class Type>
vector<Type> split(const string &str, const string &pattern)
{
	//const char* convert to char*
	char * strc = new char[strlen(str.c_str()) + 1];
	strcpy(strc, str.c_str());
	vector<Type> resultVec;
	char* tmpStr = strtok(strc, pattern.c_str());
	while (tmpStr != NULL)
	{
		string str(tmpStr);
		resultVec.push_back(stringToNum<Type>(str));
		tmpStr = strtok(NULL, pattern.c_str());
	}

	delete[] strc;

	return resultVec;
}

//要求：1.主键在首列，2.主键是数字,3.首行忽略
map<int, map<string, string> > readCsvData(string fname);

string ws2s(const wstring& ws);

string fileStr2Str(string);

string getDirString(direction);
string getETString(examType);

template <class Type>
string list2String(const list<Type> &l)
{
	list<Type> tmp = l;
	stringstream ss;
	while(! tmp.empty())
	{
		ss << tmp.front() << ", ";
		tmp.pop_front();
	}
	return ss.str();
}
#endif /* commonFun_h */

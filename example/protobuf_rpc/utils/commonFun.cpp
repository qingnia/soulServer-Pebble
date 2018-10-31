//
//  commonFun.cpp
//  testC
//
//  Created by 陈帅 on 2018/9/19.
//

#include "commonFun.h"

vector<int> myShuffle(int num)
{
	vector<int> tmpVec(num);

	for (int i = 0; i < num; i++) {
		tmpVec[i] = i + 1;
	}

	// obtain a time-based seed:  
	unsigned seed = (unsigned)chrono::system_clock::now().time_since_epoch().count();
	shuffle(tmpVec.begin(), tmpVec.end(), default_random_engine(seed));
	return tmpVec;
}

list<int> myShuffle2List(int num)
{
	vector<int> shuffleNums = myShuffle(num);

	list<int> tmpList;
	for (int i = 0; i < num; i++) {
		tmpList.push_back(shuffleNums[i]);
	}
	return tmpList;
}


direction reverseDir(direction dir)
{
	direction revDir = dirUp;
	switch (dir)
	{
	case dirUp:
		revDir = dirDown;
		break;
	case dirDown:
		revDir = dirUp;
		break;
	case dirLeft:
		revDir = dirRight;
		break;
	case dirRight:
		revDir = dirLeft;
		break;
	default:
		break;
	}
	return revDir;
}

bool in_vector(int value, vector<int> vec)
{
	vector<int>::iterator it;

	it = find(vec.begin(), vec.end(), value);
	if (it != vec.end())
	{
		return true;
	}
	return false;
}

string Trim(string& str)
{
	str.erase(0, str.find_first_not_of(" \t\r\n"));
	str.erase(str.find_last_not_of(" \t\r\n") + 1);
	return str;
}

map<int, map<string, string> > readCsvData(string fname)
{
	map<int, map<string, string> > fileInfo;
	ifstream fin(fname);
	string line;

	//统一忽略首行
	getline(fin, line);

	string field;

	//第二行是字段名
	getline(fin, line);
	istringstream sin(line);
	vector<string> keys;
	while (getline(sin, field, ','))
	{
		keys.push_back(field);
	}

	string value;
	int pKey;	//每一行的主键,主键必须是数字
	string key;		//具体的字段名
	int fieldPos;
	while (getline(fin, line))
	{
		istringstream sin(line);
		map<string, string> kv;

		fieldPos = 0;
		while (getline(sin, value, ','))
		{
			//首列是key
			if (fieldPos == 0)
			{
				pKey = stringToNum<int>(value);
			}
			kv[keys[fieldPos]] = fileStr2Str(value);
			
			fieldPos++;
		}
		fileInfo[pKey] = kv;
	}
	return fileInfo;
}


template <class Iterator, class T>
bool myFind(Iterator begin, Iterator end, const T &value)
{
	while (begin != end && *begin != value)
	{
		++begin;
	}
	if (begin != end)
	{
		return true;
	}
	return false;
}

string ws2s(const wstring& ws)
{
	string curLocale = setlocale(LC_ALL, NULL);  // curLocale = "C";

	setlocale(LC_ALL, "chs");

	const wchar_t* _Source = ws.c_str();
	size_t _Dsize = 2 * ws.size() + 1;
	char* _Dest = new char[_Dsize];
	memset(_Dest, 0, _Dsize);
	wcstombs(_Dest, _Source, _Dsize);
	string result = _Dest;
	delete []_Dest;

	setlocale(LC_ALL, curLocale.c_str());

	return result;
}

string fileStr2Str(string str)
{
	return str;
	//std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
	//wstring wb = conv.from_bytes(str);
	//return ws2s(wb);
}

string getDirString(direction dir)
{
	string str;
	switch(dir)
	{
	case dirUp:
		str = "上";
		break;
	case dirDown:
		str = "下";
		break;
	case dirLeft:
		str = "左";
		break;
	case dirRight:
		str = "右";
		break;
	default:
		str = "停";
		break;
	}
	return str;
}

string getETString(examType et)
{
	string str;
	switch (et)
	{
	case etSpeed:
		str = "速度";
		break;
	case etStrength:
		str = "力量";
		break;
	case etSpirit:
		str = "神志";
		break;
	case etKnowledge:
		str = "知识";
		break;
	case etPhysicalDamage:
		str = "物理损伤";
		break;
	case etMindDamage:
		str = "精神损伤";
		break;
	default:
		break;
	}
	return str;
}

//
//  coding.h
//  testC
//
//  Created by 陈帅 on 2018/9/26.
//

#ifndef coding_h
#define coding_h

#include <iostream>
#include <string>
#include <fstream>
#include <codecvt>
#include <stdio.h>

using namespace std;
string test()
{
	std::wstring str = L"123,abc:我是谁！";
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	std::string narrowStr = conv.to_bytes(str);
	{
		std::ofstream ofs("d:\\test.txt");	//文件是utf8编码
		ofs << narrowStr;
	}
	std::ifstream ifs(L"d:\\test.txt");
	while (!ifs.eof())
	{
		string line;
		getline(ifs, line);
		wstring wb = conv.from_bytes(line);
		wcout.imbue(locale("chs"));			//更改区域设置 只为控制台输出显示
		wcout << wb << endl;
	}
	return "";
}


#include <codecvt>
/*  解码不同编码的文本文件类*/
typedef enum tagTextCodeType
{
	TextUnkonw = -1,
	TextANSI = 0,
	TextUTF8,
	TextUNICODE,
	TextUNICODE_BIG
}TextCodeType;

class CDecodeTextFile
{
public:
	CDecodeTextFile();
	~CDecodeTextFile();
	//interface
	wstring DecodeFileToWstring(const wchar_t *filePath);
	string DecodeFileToString(const wchar_t *filePath);

private:
	TextCodeType GetFileEncodeType(const wchar_t *filePath);

	wstring ReadString(TextCodeType type);
	wstring ReadAnsiString();
	wstring ReadUtf8String();
	wstring ReadUnicodeString();
	wstring ReadUnicodeBigString();

private:
	FILE  *m_fp;
};
#endif
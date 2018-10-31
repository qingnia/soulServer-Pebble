//
//  coding.cpp
//  testC
//
//  Created by 陈帅 on 2018/9/26.
//

#include "coding.h"

//.cpp文件
CDecodeTextFile::CDecodeTextFile() :
	m_fp(NULL)
{

}

CDecodeTextFile::~CDecodeTextFile()
{
	if (m_fp)
		fclose(m_fp);
}


wstring CDecodeTextFile::DecodeFileToWstring(const wchar_t *filePath)
{
	TextCodeType type = GetFileEncodeType(filePath);
	return ReadString(type);
}

string CDecodeTextFile::DecodeFileToString(const wchar_t *filePath)
{
	string Dst = "";
	return Dst;
}

TextCodeType CDecodeTextFile::GetFileEncodeType(const wchar_t *filePath)
{
	unsigned char headBuf[3] = { 0 };
	TextCodeType type = TextUnkonw;

	m_fp = _wfopen(filePath, L"r");
	if (!m_fp) return type;

	fseek(m_fp, 0, SEEK_SET);
	fread(headBuf, 3, 1, m_fp);

	if (headBuf[0] == 0xEF && headBuf[1] == 0xBB && headBuf[2] == 0xBF)     //utf8-bom 文件开头：FF BB BF,不带bom稍后解决
		type = TextUTF8;
	else if (headBuf[0] == 0xFF && headBuf[1] == 0xFE)      //小端Unicode  文件开头：FF FE  intel x86架构自身是小端存储，可直接读取
		type = TextUNICODE;
	else if (headBuf[0] == 0xFE && headBuf[1] == 0xFF)  //大端Unicode  文件开头：FE FF
		type = TextUNICODE_BIG;
	else
		type = TextANSI;    //ansi或者unf8 无bom

	return type;
}


wstring CDecodeTextFile::ReadString(TextCodeType type)
{
	if (!m_fp) return NULL;
	switch (type)
	{
	case TextANSI:
		return ReadAnsiString();
		break;
	case TextUTF8:      //无bom 
		return ReadUtf8String();
		break;
	case TextUNICODE:
		return ReadUnicodeString();
		break;
	case TextUNICODE_BIG:
		return ReadUnicodeBigString();
		break;
	}


	return NULL;
}

wstring CDecodeTextFile::ReadAnsiString()
{
	const char* GBK_LOCALE_NAME = ".936"; //GBK在windows下的locale name

	wstring_convert<codecvt_byname<wchar_t, char, mbstate_t>> Conver_GBK(new codecvt_byname<wchar_t, char, mbstate_t>(GBK_LOCALE_NAME));    //GBK - whar

	fseek(m_fp, 0, SEEK_END);
	int fileSize = ftell(m_fp);
	char *buf = new char[fileSize + 1];
	memset(buf, 0, fileSize + 1);

	fseek(m_fp, 0, SEEK_SET);
	fread(buf, sizeof(char), fileSize, m_fp);

	wstring wDst = Conver_GBK.from_bytes(buf);      //GBK转Unicode

	delete buf;
	return wDst;
}
wstring CDecodeTextFile::ReadUtf8String()
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> Conver_UTF8;   //utf8-wchar

	char  headBuf[3] = { 0 };

	fseek(m_fp, 0, SEEK_END);
	int fileSize = ftell(m_fp);

	fseek(m_fp, 0, SEEK_SET);
	fread(headBuf, sizeof(char), 3, m_fp);
	if (headBuf[0] == 0xEF && headBuf[1] == 0xBB && headBuf[2] == 0xBF)     //带 Bom
		fseek(m_fp, 3, SEEK_SET);
	else
		fseek(m_fp, 0, SEEK_SET);

	char *buf = new char[fileSize];
	memset(buf, 0, fileSize);

	fseek(m_fp, 0, SEEK_SET);
	fread(buf, sizeof(char), fileSize, m_fp);

	wstring wDst = Conver_UTF8.from_bytes(buf);     //GBK转Unicode

	delete buf;
	return wDst;

}
wstring CDecodeTextFile::ReadUnicodeString()
{
	fseek(m_fp, 0, SEEK_END);
	int fileSize = ftell(m_fp);

	wchar_t *buf = new wchar_t[fileSize / 2];
	memset((void *)buf, 0, fileSize);

	fseek(m_fp, 2, SEEK_SET);
	fread(buf, sizeof(char), fileSize, m_fp);

	wstring wDst = buf;

	delete buf;
	return wDst;
}
wstring CDecodeTextFile::ReadUnicodeBigString()
{
	wstring wDst = L"";
	return wDst;
}
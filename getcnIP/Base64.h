#pragma once
#ifndef ___BASE64_H___
#define ___BASE64_H___

#include <string>

using namespace std;
/*
 * 1.加密输入的是UTF-8字符串
 * 2.解密输出的是UTF-8字符串，可以转化成wstring在控制台输出或直接输出文件
 */
class Base64
{
public:
	Base64();
	~Base64();
	string static Encode(string u8str);
	string static Decode(const string& strIn);
};

#endif // ___BASE64_H___
#include "Base64.h"

static const char base64EncodeChars[] =
"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static const char base64DecodeChars[] =
{
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255,  62, 255, 255, 255,  63,
	52,  53,  54,  55,  56,  57,  58,  59,  60,  61, 255, 255,
	255, 254, 255, 255, 255,   0,   1,   2,   3,   4,   5,   6,
	7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,
	19,  20,  21,  22,  23,  24,  25, 255, 255, 255, 255, 255,
	255,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,
	37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,
	49,  50,  51, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
	255, 255, 255, 255
};

Base64::Base64()
{
}

Base64::~Base64()
{
}
string Base64::Encode(string u8str)
{
	size_t len = u8str.length();
	string strOut = "";
	for(size_t i = 0;i < len;) {
		wchar_t c1 = u8str[i++] & 0xff;
		if(i == len)
		{
			strOut += base64EncodeChars[c1 >> 2];
			strOut += base64EncodeChars[(c1 & 0x3) << 4];
			strOut += "==";
			break;
		}
		wchar_t c2 = u8str[i++];
		if(i == len)
		{
			strOut += base64EncodeChars[c1 >> 2];
			strOut += base64EncodeChars[((c1 & 0x3)<< 4) | ((c2 & 0xF0) >> 4)];
			strOut += base64EncodeChars[(c2 & 0xF) << 2];
			strOut += "=";
			break;
		}
		wchar_t c3 = u8str[i++];
		strOut += base64EncodeChars[c1 >> 2];
		strOut += base64EncodeChars[((c1 & 0x3)<< 4) | ((c2 & 0xF0) >> 4)];
		strOut += base64EncodeChars[((c2 & 0xF) << 2) | ((c3 & 0xC0) >>6)];
		strOut += base64EncodeChars[c3 & 0x3F];
    }
	return strOut;
}

string Base64::Decode(const string& strIn)
{
	unsigned long t, y;
	unsigned long g = 3;
	string u8str = "";

	for (unsigned long x = y = t = 0; x < strIn.length(); ++x)
	{
		unsigned char c = base64DecodeChars[strIn[x]];
		if (c == 255) 
			continue;
		if (c == 254)
		{
			c = 0; 
			--g;
		}

		t = (t << 6) | c;

		if (++y == 4)
		{
			u8str += static_cast<unsigned char>((t >> 16) & 255);
			if (g > 1) 
				u8str += static_cast<unsigned char>((t >> 8) & 255);
			if (g > 2) 
				u8str += static_cast<unsigned char>(t & 255);
			y = t = 0;
		}
	}
	return u8str;
}
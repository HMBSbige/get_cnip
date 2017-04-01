#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <io.h>
#include <direct.h>  
using namespace std;
typedef long long LL;
/////////////////////////////////////////////////////////////////////////////
struct IPList
{
	string IP, Mask,CNIPv4;
	IPList(){}
	IPList(string a, string b):IP(a), Mask(b){}
};
inline LL StringToNum(const string& s) {
	LL num;
	stringstream ss(s);
	ss >> num;
	return num;
}
inline string NumToString(const LL& i) {
	ostringstream oss;
	oss << i;
	return oss.str();
}
string getSubnetMask(LL num) {
	string mask[4];
	LL n[4] = { 255,255,255,0 };
	if (num > (1 << 24)) {
		n[0] -= (num >> 24) - 1;
		n[1] = n[2] = 0;
	}
	else if (num > (1 << 16)) {
		n[1] -= (num >> 16) - 1;
		n[2] = 0;
	}
	else if (num > (1 << 8))
		n[2] -= (num >> 8) - 1;
	for (int i = 0; i < 4; ++i)
		mask[i] = NumToString(n[i]);
	return mask[0] + "." + mask[1] + "." + mask[2] + "." + mask[3];
}
IPList getIP(string line) {
	string temp(line, 14, line.length() - 14);
	size_t pos1 = temp.find("|");
	string IP(temp, 0, pos1);
	size_t pos2 = temp.find("|", pos1 + 1);
	string Mask(temp, pos1 + 1, pos2 - pos1 - 1);
	Mask = getSubnetMask(StringToNum(Mask));
	return IPList(IP, Mask);
}
bool isCNipv4(string line) {
	string temp(line, 0, 14);
	if (temp == "apnic|CN|ipv4|")
		return true;
	return false;
}
/////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[]) {
	std::ios::sync_with_stdio(false);
	setlocale(LC_ALL, "");
	string PATH = ".\\", filename = "delegated-apnic-latest.txt";
	if (argc != 1) {
		string temp= argv[1];
		size_t x = temp.rfind("\\");
		PATH.assign(temp,0,x+1);
		filename.assign(temp,x+1);
	}
	else {
		cout << "程序以默认参数运行..." << endl;
	}
	cout << "文件路径：" << PATH + filename << endl;
/////////////////////////////////////////////////////////
	ifstream rawdata(PATH + filename);
	if (rawdata) {
		ofstream outfile, add, del, ssr;
		string tempStr, IP, mask;
		register size_t i = 0;
		vector<IPList> IP_Mask;

		if (_access((PATH + "\\cnIP").c_str(), 0) == -1)//判断文件夹是否存在
			_mkdir((PATH + "\\cnIP").c_str());//system(("md " + PATH + "\\cnIP").c_str());
		outfile.open(PATH + "\\cnIP\\" + "cnIP.txt", ios::trunc);
		add.open(PATH + "\\cnIP\\" + "add.txt", ios::trunc);
		del.open(PATH + "\\cnIP\\" + "del.txt", ios::trunc);
		ssr.open(PATH + "\\cnIP\\" + "delegated-apnic.txt", ios::trunc);

		cout << "正在分析路由表..." << endl;
		while (getline(rawdata, tempStr, '\n')) {
			if (isCNipv4(tempStr)) {
				IPList tempList=getIP(tempStr);
				tempList.CNIPv4 = tempStr;
				IP_Mask.push_back(tempList);
			}
		}
		cout << "正在生成路由表..." << endl;
		for (i = 0;i<IP_Mask.size();++i) {
				if (i) {
					outfile << endl;
					add << endl;
					del << endl;
					ssr << endl;
				}
				IP = IP_Mask.at(i).IP;
				mask = IP_Mask.at(i).Mask;
				outfile << IP + "|" + mask;
				add << "add " + IP + " mask " + mask + " default METRIC default IF default";
				del << "delete " + IP + " mask " + mask + " default METRIC default IF default";
				ssr << IP_Mask.at(i).CNIPv4;
		}
		outfile.close();
		add.close();
		del.close();
		ssr.close();
		cout<<"路由表生成成功！共有"<< IP_Mask.size() << "条。" <<endl;
	}
	else {
		cout << "打开文件 "+ PATH + filename +" 错误。" << endl<<"请检查正确的路径或确定程序在 "+ PATH +" 目录是否有权限！"<< endl;
	}

	cout << endl;
	system("pause");
	return 0;
}
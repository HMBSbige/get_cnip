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
class IPv4 {
private:
	vector<LL> n;
public:
	IPv4(const IPv4& a) :n(4)
	{
		n[0] = a.n[0];
		n[1] = a.n[1];
		n[2] = a.n[2];
		n[3] = a.n[3];
	}
	IPv4(LL n1=0, LL n2=0, LL n3=0, LL n4=0) :n(4)
	{
		n[0] = n1;
		n[1] = n2;
		n[2] = n3;
		n[3] = n4;
	}
	IPv4(string IP):n(4)
	{
		getIPv4(IP);
	}
	IPv4(vector<LL> a)
	{
		n = a;
	}
	void getIPv4(string IP) {
		size_t pos1 = IP.find(".");
		n[0] = StringToNum(string(IP, 0, pos1));
		size_t pos2 = IP.find(".", pos1 + 1);
		n[1] = StringToNum(string(IP, pos1 + 1, pos2 - pos1 - 1));
		pos1 = IP.find(".", pos2 + 1);
		n[2] = StringToNum(string(IP, pos2 + 1, pos1 - pos2 - 1));
		pos2 = IP.size();
		n[3] = StringToNum(string(IP, pos1 + 1, pos2 - pos1 - 1));
	}
	LL getIPv4(size_t i) {
		return this->n.at(i);
	}
	vector<LL> getIPv4() {
		return n;
	}
	string str() {
		return NumToString(n[0]) + "." + NumToString(n[1]) + "." + NumToString(n[2]) + "." + NumToString(n[3]);
	}
};
struct IPList
{
	string Mask,CNIPv4;
	IPv4 FirstIP;
	IPv4 LastIP;
	IPList() {}
	IPList(string a, string b,string c):FirstIP(a), Mask(b), LastIP(c){}

};
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
string getLastIP(string IP, LL num) {
	register size_t i = 2;
	while (num / 256 > 256) {
		--i;
		num /= 256;
	}
	num /= 256;
	--num;
	vector<LL> n = IPv4(IP).getIPv4();
	n[i] += num;
	for (++i; i < 4; ++i)
		n[i] = 255;
	return IPv4(n).str();
}
IPList getIP(string line) {
	string temp(line, 14, line.length() - 14);
	size_t pos1 = temp.find("|");
	string IP(temp, 0, pos1);
	size_t pos2 = temp.find("|", pos1 + 1);
	string NumberofIP(temp, pos1 + 1, pos2 - pos1 - 1);
	string Mask = getSubnetMask(StringToNum(NumberofIP));
	string LastIP = getLastIP(IP, StringToNum(NumberofIP));
	return IPList(IP, Mask, LastIP);
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
		ofstream outfile, add, del, ssr,newssr;
		string tempStr, IP, mask;
		register size_t i = 0;
		vector<IPList> IP_Mask;
	
		if (_access((PATH + "\\cnIP").c_str(), 0) == -1)//判断文件夹是否存在
			_mkdir((PATH + "\\cnIP").c_str());//system(("md " + PATH + "\\cnIP").c_str());
		outfile.open(PATH + "\\cnIP\\" + "cnIP.txt", ios::trunc);
		add.open(PATH + "\\cnIP\\" + "add.txt", ios::trunc);
		del.open(PATH + "\\cnIP\\" + "del.txt", ios::trunc);
		ssr.open(PATH + "\\cnIP\\" + "delegated-apnic.txt", ios::trunc);
		newssr.open(PATH + "\\cnIP\\" + "chn_ip.txt", ios::trunc);

		cout << "正在分析路由表..." << endl;
		while (getline(rawdata, tempStr, '\n')) {
			if (isCNipv4(tempStr)) {
				IPList tempList = getIP(tempStr);
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
					newssr << endl;
				}
				IP = IP_Mask.at(i).FirstIP.str();
				mask = IP_Mask.at(i).Mask;
				outfile << IP + "|" + mask;
				add << "add " + IP + " mask " + mask + " default METRIC default IF default";
				del << "delete " + IP + " mask " + mask + " default METRIC default IF default";
				ssr << IP_Mask.at(i).CNIPv4;
				newssr << IP_Mask.at(i).FirstIP.str() <<" "<< IP_Mask.at(i).LastIP.str();
		}
		outfile.close();
		add.close();
		del.close();
		ssr.close();
		newssr.close();
		cout<<"路由表生成成功！共有"<< IP_Mask.size() << "条。" <<endl;
	}
	else {
		cout << "打开文件 "+ PATH + filename +" 错误。" << endl<<"请检查正确的路径或确定程序在 "+ PATH +" 目录是否有权限！"<< endl;
	}
	
	cout << endl;
	system("pause");
	return 0;
}
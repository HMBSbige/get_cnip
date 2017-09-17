#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <io.h>
#include <direct.h>
#include "IPv4.h"
using namespace std;
/////////////////////////////////////////////////////////////////////////////


struct ip_list
{
	IPv4 first_ip;
	IPv4 last_ip;
	string mask, cn_ipv4;
	ip_list() {}
	ip_list(string a, string b,string c):first_ip(a), last_ip(c), mask(b){}

};
string getSubnetMask(ll num) {
	string mask[4];
	ll n[4] = { 255,255,255,0 };
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
		mask[i] = num_to_string(n[i]);
	return mask[0] + "." + mask[1] + "." + mask[2] + "." + mask[3];
}
string getLastIP(string IP, ll num) {
	size_t i = 2;
	while (num / 256 > 256) {
		--i;
		num /= 256;
	}
	num /= 256;
	--num;
	vector<ll> n = IPv4(IP).get_ipv4();
	n[i] += num;
	for (++i; i < 4; ++i)
		n[i] = 255;
	return IPv4(n).str();
}
ip_list getIP(string line) {
	string temp(line, 14, line.length() - 14);
	const auto pos1 = temp.find("|");
	const string ip(temp, 0, pos1);
	const auto pos2 = temp.find("|", pos1 + 1);
	const string number_of_ip(temp, pos1 + 1, pos2 - pos1 - 1);
	const auto mask = getSubnetMask(string_to_num(number_of_ip));
	const auto last_ip = getLastIP(ip, string_to_num(number_of_ip));
	return ip_list(ip, mask, last_ip);
}
bool is_cn_ipv4(string line) {
	const string temp(line, 0, 14);
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
		const auto x = temp.rfind("\\");
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
		string temp_str;
		vector<ip_list> ip_mask;
	
		if (_access((PATH + "\\cnIP").c_str(), 0) == -1)//判断文件夹是否存在
			_mkdir((PATH + "\\cnIP").c_str());//system(("md " + PATH + "\\cnIP").c_str());
		outfile.open(PATH + "\\cnIP\\" + "cnIP.txt", ios::trunc);
		add.open(PATH + "\\cnIP\\" + "add.txt", ios::trunc);
		del.open(PATH + "\\cnIP\\" + "del.txt", ios::trunc);
		ssr.open(PATH + "\\cnIP\\" + "delegated-apnic.txt", ios::trunc);
		newssr.open(PATH + "\\cnIP\\" + "chn_ip.txt", ios::trunc);

		cout << "正在分析路由表..." << endl;
		while (getline(rawdata, temp_str, '\n')) {
			if (is_cn_ipv4(temp_str)) {
				auto temp_list = getIP(temp_str);
				temp_list.cn_ipv4 = temp_str;
				ip_mask.push_back(temp_list);
			}
		}
		cout << "正在生成路由表..." << endl;
		for (auto i = 0;i<ip_mask.size();++i) {
			if (i) {
				outfile << endl;
				add << endl;
				del << endl;
				ssr << endl;
				newssr << endl;
			}
			const auto ip = ip_mask.at(i).first_ip.str();
			const auto mask = ip_mask.at(i).mask;
			outfile << ip + "|" + mask;
			add << "add " + ip + " mask " + mask + " default METRIC default IF default";
			del << "delete " + ip + " mask " + mask + " default METRIC default IF default";
			ssr << ip_mask.at(i).cn_ipv4;
			newssr << ip_mask.at(i).first_ip.str() <<" "<< ip_mask.at(i).last_ip.str();
		}
		outfile.close();
		add.close();
		del.close();
		ssr.close();
		newssr.close();
		cout<<"路由表生成成功！共有"<< ip_mask.size() << "条。" <<endl;
	}
	else {
		cout << "打开文件 "+ PATH + filename +" 错误。" << endl<<"请检查正确的路径或确定程序在 "+ PATH +" 目录是否有权限！"<< endl;
	}
	
	cout << endl;
	system("pause");
	return 0;
}
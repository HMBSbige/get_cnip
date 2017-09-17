#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <io.h>
#include <direct.h>
#include "ipv4.h"
using namespace std;
/////////////////////////////////////////////////////////////////////////////
void getcnip()
{
	const string filename = R"(delegated-apnic-latest.txt)";

	ifstream rawdata(filename);
	if (rawdata) {
		ofstream outfile, add, del,ssr;
		string temp_str;
		vector<ip_list> ip_mask;

		if (_access(string(R"(.\cnIP)").c_str(), 0) == -1)//判断文件夹是否存在
			_mkdir(string(R"(.\cnIP)").c_str());//system(("md " + PATH + "\\cnIP").c_str());
		outfile.open(R"(.\cnIP\cnIP.txt)", ios::trunc);
		add.open(R"(.\cnIP\add.txt)", ios::trunc);
		del.open(R"(.\cnIP\del.txt)", ios::trunc);
		ssr.open(R"(.\cnIP\chn_ip.txt)", ios::trunc);

		cout << "正在分析路由表..." << endl;
		while (getline(rawdata, temp_str, '\n')) {
			if (is_cn_ipv4(temp_str)) {
				auto temp_list = get_ip(temp_str);
				temp_list.cn_ipv4 = temp_str;
				ip_mask.push_back(temp_list);
			}
		}
		cout << "正在生成路由表..." << endl;
		for (auto i = 0; i < ip_mask.size(); ++i) {
			if (i) {
				outfile << endl;
				add << endl;
				del << endl;
				ssr << endl;
			}
			const auto ip = ip_mask.at(i).first_ip.str();
			const auto mask = ip_mask.at(i).mask;
			outfile << ip + "|" + mask;
			add << "add " + ip + " mask " + mask + " default METRIC default IF default";
			del << "delete " + ip + " mask " + mask + " default METRIC default IF default";
			ssr << ip_mask.at(i).first_ip.str() << " " << ip_mask.at(i).last_ip.str();
		}
		outfile.close();
		add.close();
		del.close();
		ssr.close();
		cout << "路由表生成成功！共有" << ip_mask.size() << "条。" << endl;
	}
	else {
		cout << "未找到 " + filename << endl;
	}
}
void gfwlist2pac()
{
	
}
void get_cn_domains()
{
	
}
int main() {
	std::ios::sync_with_stdio(false);
	setlocale(LC_ALL, "");
	getcnip();
	gfwlist2pac();
	get_cn_domains();
	cout << endl;
	system("pause");
	return 0;
}
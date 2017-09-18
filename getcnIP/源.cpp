#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <io.h>
#include <direct.h>
#include <sstream>
#include <queue>
#include "ipv4.h"
#include "Base64.h"
using namespace std;
/////////////////////////////////////////////////////////////////////////////
void getcnip()
{
	const string filename = R"(delegated-apnic-latest.txt)";

	ifstream rawdata(filename);
	if (rawdata) {
		ofstream add, del,ssr;
		string temp_str;
		vector<ip_list> ip_mask;

		cout << "找到 " + filename << endl;
		if (_access(string(R"(.\out)").c_str(), 0) == -1)//判断文件夹是否存在
		{
			_mkdir(string(R"(.\out)").c_str());//system(("md " + PATH + "\\out").c_str());
		}	
		add.open(R"(.\out\add.txt)", ios::trunc);
		del.open(R"(.\out\del.txt)", ios::trunc);
		ssr.open(R"(.\out\chn_ip.txt)", ios::trunc);

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
				add << endl;
				del << endl;
				ssr << endl;
			}
			const auto ip = ip_mask.at(i).first_ip.str();
			const auto mask = ip_mask.at(i).mask;
			add << "add " + ip + " mask " + mask + " default METRIC default IF default";
			del << "delete " + ip + " mask " + mask + " default METRIC default IF default";
			ssr << ip_mask.at(i).first_ip.str() << " " << ip_mask.at(i).last_ip.str();
		}


		add.close();
		del.close();
		ssr.close();
		cout << "路由表生成成功！共有" << ip_mask.size() << "条。" << endl;
	}
	else {
		cout << "未找到 " + filename << endl;
	}
	cout << endl;
}
string textfile2str(const string& PATH)
{
	ifstream file(PATH);
	ostringstream oss;
	oss << file.rdbuf();
	file.close();
	return oss.str();
}
string& replace_all_distinct(string& str,const string& old_value,const string& new_value)
{
	for (string::size_type pos(0); pos != string::npos; pos += new_value.length()) {
		if ((pos = str.find(old_value, pos)) != string::npos)
			str.replace(pos, old_value.length(), new_value);
		else
			break;
	}
	return str;
}
void gfwlist2pac()
{
	const string gfwlist = R"(gfwlist.txt)";
	const string userrule = R"(user-rule.txt)";

	const ifstream gfwdata(gfwlist);

	if(gfwdata)
	{
		cout << "找到 " + gfwlist << endl;
		auto list = Base64::Decode(textfile2str(gfwlist));//decode gfwlist
		const ifstream userdata(userrule);
		if (userdata)
		{
			cout << "找到 " + userrule << endl;
			list += textfile2str(userrule);
		}
		else
		{
			cout << "未找到 " + userrule << endl;
		}

		cout << "正在分析GFWList..." << endl;
		string temp_str;
		istringstream ss_list(list);
		queue<string> domains;

		while(getline(ss_list,temp_str,'\n'))
		{
			if (temp_str[0] != '!' && temp_str!="" && temp_str[0] != '[')
			{
				temp_str = replace_all_distinct(temp_str, R"(\)", R"(\\)");//将"\"替换成"\\"
				domains.push(temp_str);
			}
				
		}
		cout << "共有"<<domains.size()<<"条。" << endl;

		//output
		cout << "正在生成pac..." << endl;
		ofstream pac;
		pac.open(R"(.\out\pac.txt)", ios::trunc);
		pac << pac_front_str;
		while(!domains.empty())
		{
			if(domains.size()!=1)
				pac<<"\t\""+domains.front()+"\","<<endl;
			else
				pac << "\t\"" + domains.front() + "\"" << endl;
			domains.pop();
		}
		pac << pac_back_str1;
		pac << pac_back_str2;
		pac.close();
		cout << "pac生成成功！"<< endl;
	}
	else 
	{
		cout << "未找到 " + gfwlist << endl;
	}
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
	//system("pause");
	return 0;
}
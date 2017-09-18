#include <iostream>

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
		cout << "正在生成pac.txt..." << endl;
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
		cout << "pac.txt生成成功！"<< endl;
	}
	else 
	{
		cout << "未找到 " + gfwlist << endl;
	}
	cout << endl;
}
void get_cn_domains()
{
	const string rawdata_name = R"(accelerated-domains.china.conf)";
	const string whitelist = R"(whitelist.txt)";
	queue<string> domains;
	string temp_str;

	ifstream domains_data(rawdata_name);
	if (domains_data)
	{
		cout << "找到 " + rawdata_name << endl;
		ifstream whitelist_data(whitelist);
		if (whitelist_data)
		{
			cout << "找到 " + whitelist << endl;
			while(getline(whitelist_data,temp_str,'\n'))
			{
				if(temp_str!="")
				domains.push(temp_str);
			}
			cout << "用户自定义域名共" << domains.size() << "条。" << endl;
		}
		else
		{
			cout << "未找到 " + whitelist << endl;
		}

		cout << "正在分析 accelerated-domains.china.conf ..." << endl;

		while (getline(domains_data, temp_str, '\n'))
		{
			if (temp_str != "")
			{
				//取出域名
				auto pos1 = temp_str.find('/');
				auto pos2 = temp_str.rfind('/');
				domains.push("*." + temp_str.substr(pos1+1,pos2-pos1-1));
			}
		}
		cout << "共有" << domains.size() << "条。" << endl;

		//output
		cout << "正在生成CNdomains.txt..." << endl;
		ofstream cn_domains;
		cn_domains.open(R"(.\out\CNdomains.txt)", ios::trunc);
		while (!domains.empty())
		{
			cn_domains <<domains.front()<< endl;
			domains.pop();
		}
		cn_domains.close();
		cout << "CNdomains.txt生成成功！" << endl;
	}
	else
	{
		cout << "未找到 " + rawdata_name << endl;
	}
	cout << endl;
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
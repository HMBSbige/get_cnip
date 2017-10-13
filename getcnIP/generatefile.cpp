#include "generatefile.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <string>
#include "ipv4.h"
#include "Base64.h"
#include "misc.h"

using namespace std;

queue<string> q_user_dot_rule_local_proxy;
queue<string> q_user_dot_rule_ip_rules;
queue<string> q_white_domains;
vector<pair<ll, ll> > lines_list;
string white_domains="";
string cnIpRange="";
string cnIp16Range = "";

void make_cnIpRange(string& list_result)
{
	list_result= "[\n{";
	string temp_str;

	ll start_num = 0;
	auto comma = "";
	for (auto&line : lines_list)
	{
		while (line.first >> 24 > start_num) {
			start_num += 1;
			list_result.append("},{");
			comma = "";
		}
		list_result.append(comma);
		list_result.append("0x");
		list_result.append(NumberBaseConversion(10, 16, to_string(line.first / 256)));
		list_result.append(":");
		list_result.append(to_string(line.second / 256));
		comma = ",";
	}
	list_result.append("}\n];\n");
}
void make_cnIp16Range(string& list_result)
{
	list_result = "{\n";
	string temp_str;
	vector<ll> master_net;
	set<ll> master_net_set;

	for (auto&line : lines_list)
	{
		if (line.second < 1 << 14) {
			master_net_set.insert(line.first >> 14);
		}
	}
	for (auto&x : master_net_set)
	{
		master_net.push_back(x);
	}
	sort(master_net.begin(), master_net.end());
	for (auto&s : master_net)
	{
		list_result.append("0x");
		list_result.append(NumberBaseConversion(10, 16, to_string(s)));
		list_result.append(":1,");
	}
	list_result.erase(list_result.end() - 1);
	list_result.append("\n};\n\n");
}
multimap<string,string> white_domains_queue2dict(queue<string>& q)
{
	multimap<string, string> dict;
	while(!q.empty())
	{
		dict.insert(rsplit_dot(q.front()));
		q.pop();
	}
	return dict;
}
void make_white_domains(string& result)
{
	result="";
	auto white_domains_dict = white_domains_queue2dict(q_white_domains);
	pair<multimap<string, string>::iterator, multimap<string, string>::iterator> it_range;
	auto is_first = true;
	for (auto it = white_domains_dict.begin(); it != white_domains_dict.end(); it = it_range.second)
	{
		it_range = white_domains_dict.equal_range(it->first);
		if (it->first == "")
		{
			continue;
		}
		if (!is_first)
		{
			result += R"(,)";
		}
		result += R"(")" + it->first + R"(":{
)";
		is_first = false;
		const auto temp_it = it_range.first;
		while (it_range.first != it_range.second)
		{
			if (temp_it != it_range.first)
			{
				result += R"(,
)";
			}
			result += R"(")" + it_range.first->second + R"(":1)";
			++it_range.first;
		}
		result += R"(
})";
	}
}
void getcnip()
{
	const string filename = R"(delegated-apnic-latest)";

	ifstream rawdata(filename);
	if (rawdata) {
		ofstream add, del, ssr;
		string temp_str;
		queue<ip_list> cn_ip_queue;
		ofstream test;

		cout << "找到 " + filename << endl;

		add.open(R"(.\out\add.txt)", ios::trunc);
		del.open(R"(.\out\del.txt)", ios::trunc);
		ssr.open(R"(.\out\chn_ip.txt)", ios::trunc);
		test.open(R"(.\out\test.txt)", ios::trunc);

		cout << "正在分析路由表..." << endl;

		while (getline(rawdata, temp_str, '\n')) {
			if (is_cn_ipv4(temp_str)) {
				cn_ip_queue.push(get_ip(temp_str));
			}
		}

		cout << "正在生成路由表..." << endl;

		const auto number_of_ip = cn_ip_queue.size();
		for (; !cn_ip_queue.empty(); cn_ip_queue.pop()) {
			const auto ip = cn_ip_queue.front().first_ip.str();
			const auto mask = cn_ip_queue.front().mask;
			add << "add " + ip + " mask " + mask + " default METRIC default IF default" << endl;
			del << "delete " + ip + " mask " + mask + " default METRIC default IF default" << endl;
			temp_str = cn_ip_queue.front().first_ip.str() + " " + cn_ip_queue.front().last_ip.str();
			ssr << temp_str << endl;
			test << cn_ip_queue.front().first_ip.ip_to_long() <<" "<<cn_ip_queue.front().Hosts << endl;

			q_user_dot_rule_ip_rules.push(temp_str);
			lines_list.push_back(make_pair(cn_ip_queue.front().first_ip.ip_to_long(), cn_ip_queue.front().Hosts));
		}
		add.close();
		del.close();
		ssr.close();
		test.close();
		cout << "路由表生成成功！共有" << number_of_ip << "条。" << endl;
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

	if (gfwdata)
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

		while (getline(ss_list, temp_str, '\n'))
		{
			if (temp_str[0] != '!' && temp_str != "" && temp_str[0] != '[')
			{
				replace_all_distinct(temp_str, R"(\)", R"(\\)");//将"\"替换成"\\"
				domains.push(temp_str);
			}

		}
		cout << "共有" << domains.size() << "条。" << endl;

		//output
		cout << "正在生成pac.txt..." << endl;
		ofstream pac;
		pac.open(R"(.\out\gfwlist.pac)", ios::trunc);
		pac << pac_front_str;
		while (!domains.empty())
		{
			if (domains.size() != 1)
				pac << "\t\"" + domains.front() + "\"," << endl;
			else
				pac << "\t\"" + domains.front() + "\"" << endl;
			domains.pop();
		}
		pac << pac_back_str1;
		pac << pac_back_str2;
		pac.close();
		cout << "pac.txt生成成功！" << endl;
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
			while (getline(whitelist_data, temp_str, '\n'))
			{
				if (temp_str != "")
				{
					domains.push(temp_str);
					replace_all_distinct(temp_str, R"(*.)", R"(.)");//将"*."替换成"."
					q_user_dot_rule_local_proxy.push(temp_str);
					if (temp_str[0] == '.')
						temp_str.erase(temp_str.begin());
					q_white_domains.push(temp_str);
				}
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
				const auto pos1 = temp_str.find('/');
				const auto pos2 = temp_str.rfind('/');
				temp_str = temp_str.substr(pos1 + 1, pos2 - pos1 - 1);

				q_user_dot_rule_local_proxy.push("." + temp_str);
				domains.push("*." + temp_str);
				q_white_domains.push(temp_str);
			}
		}
		cout << "共有" << domains.size() << "条。" << endl;

		//output
		cout << "正在生成CNdomains.txt..." << endl;
		ofstream cn_domains;
		cn_domains.open(R"(.\out\CNdomains.txt)", ios::trunc);
		while (!domains.empty())
		{
			cn_domains << domains.front() << endl;
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
void generate_user_dot_rule()
{
	if (q_user_dot_rule_ip_rules.empty() && q_user_dot_rule_local_proxy.empty()) {
		cout << "未找到 accelerated-domains.china.conf 或 delegated-apnic-latest" << endl;
		return;
	}
	cout << "正在生成user.rule..." << endl;
	ofstream user_dot_rule;
	user_dot_rule.open(R"(.\out\user.rule)", ios::trunc);
	user_dot_rule << user_dot_rule_front;
	//output remote proxy rule
	user_dot_rule << user_dot_rule_remote_proxy;

	//output local proxy rule
	user_dot_rule << user_dot_rule_local_proxy;
	while (!q_user_dot_rule_local_proxy.empty())
	{
		user_dot_rule << q_user_dot_rule_local_proxy.front() << " localproxy" << endl;
		q_user_dot_rule_local_proxy.pop();
	}
	user_dot_rule << endl;

	//output direct
	user_dot_rule << user_dot_rule_direct;

	//output reject
	user_dot_rule << user_dot_rule_reject;

	//output host list
	user_dot_rule << user_dot_rule_host;
	user_dot_rule << R"(localhost 127.0.0.1)" << endl;

	//output special rule
	user_dot_rule << user_dot_rule_special;

	//output ip rules
	user_dot_rule << user_dot_rule_ip;
	while (!q_user_dot_rule_ip_rules.empty())
	{
		user_dot_rule << q_user_dot_rule_ip_rules.front() << " localproxy" << endl;
		q_user_dot_rule_ip_rules.pop();
	}
	user_dot_rule << endl;
	user_dot_rule << user_dot_rule_local;
	//output end
	user_dot_rule.close();
	cout << "user.rule生成成功！" << endl;
	cout << endl;
}
void generate_ss_cnip()
{
	if (q_white_domains.empty() || lines_list.empty()) {
		cout << "未找到 accelerated-domains.china.conf 和 delegated-apnic-latest" << endl;
		return;
	}
	
	ofstream ss_cnip_dot_pac;
	ss_cnip_dot_pac.open(R"(.\out\ss_cnip.pac)", ios::trunc);
	cout << "正在生成ss_cnip.pac..." << endl;

	ss_cnip_dot_pac << ss_cnip_front;
	//output cnIpRange
	make_cnIpRange(cnIpRange);
	ss_cnip_dot_pac << ss_cnip_cnIpRange << cnIpRange;
	//output cnIp16Range
	make_cnIp16Range(cnIp16Range);
	ss_cnip_dot_pac << ss_cnip_cnIp16Range<< cnIp16Range;
	//output whiteIpList
	ss_cnip_dot_pac << ss_cnip_whiteIpList;
	//output subnetIpRangeList
	ss_cnip_dot_pac << ss_cnip_subnetIpRangeList;
	//output white_domains
	make_white_domains(white_domains);
	ss_cnip_dot_pac <<R"(var white_domains = {)"<<white_domains<<R"(};

)";
	ss_cnip_dot_pac << ss_cnip_back;
	ss_cnip_dot_pac.close();
	cout << "ss_cnip.pac生成成功！" << endl;
	cout << endl;
}
void generate_ss_white()
{
	if (cnIpRange=="") {
		return;
	}

	ofstream ss_white_dot_pac;
	ss_white_dot_pac.open(R"(.\out\ss_white.pac)", ios::trunc);
	cout << "正在生成ss_white.pac..." << endl;

	ss_white_dot_pac << ss_cnip_front;
	//output cnIpRange
	ss_white_dot_pac << ss_cnip_cnIpRange << cnIpRange;
	//output cnIp16Range
	ss_white_dot_pac << ss_cnip_cnIp16Range << cnIp16Range;
	//output whiteIpList
	ss_white_dot_pac << ss_cnip_whiteIpList;
	//output subnetIpRangeList
	ss_white_dot_pac << ss_cnip_subnetIpRangeList;
	//output white_domains
	ss_white_dot_pac << R"(var white_domains = {)" << white_domains << R"(};

)";
	ss_white_dot_pac << ss_white_back;
	ss_white_dot_pac.close();
	cout << "ss_white.pac生成成功！" << endl;
	cout << endl;
}
void generate_ss_white_r()
{
	if (cnIpRange == "") {
		return;
	}

	ofstream ss_white_r_dot_pac;
	ss_white_r_dot_pac.open(R"(.\out\ss_white_r.pac)", ios::trunc);
	cout << "正在生成ss_white_r.pac..." << endl;

	ss_white_r_dot_pac << ss_white_r_front;
	//output cnIpRange
	ss_white_r_dot_pac << ss_cnip_cnIpRange << cnIpRange;
	//output cnIp16Range
	ss_white_r_dot_pac << ss_cnip_cnIp16Range << cnIp16Range;
	//output whiteIpList
	ss_white_r_dot_pac << ss_cnip_whiteIpList;
	//output subnetIpRangeList
	ss_white_r_dot_pac << ss_cnip_subnetIpRangeList;
	//output white_domains
	ss_white_r_dot_pac << R"(var white_domains = {)" << white_domains << R"(};

)";
	ss_white_r_dot_pac << ss_white_back;
	ss_white_r_dot_pac.close();
	cout << "ss_white_r.pac生成成功！" << endl;
	cout << endl;
}
void generate_local_cnip()
{
	if (cnIpRange == "") {
		return;
	}

	ofstream local_cnip_dot_pac;
	local_cnip_dot_pac.open(R"(.\out\local_cnip.pac)", ios::trunc);
	cout << "正在生成local_cnip.pac..." << endl;

	local_cnip_dot_pac << local_cnip_front;
	//output cnIpRange
	local_cnip_dot_pac << ss_cnip_cnIpRange << cnIpRange;
	//output cnIp16Range
	local_cnip_dot_pac << ss_cnip_cnIp16Range << cnIp16Range;
	//output whiteIpList
	local_cnip_dot_pac << ss_cnip_whiteIpList;
	//output subnetIpRangeList
	local_cnip_dot_pac << ss_cnip_subnetIpRangeList;
	//output white_domains
	local_cnip_dot_pac << R"(var white_domains = {)" << white_domains << R"(};

)";
	local_cnip_dot_pac << ss_white_back;
	local_cnip_dot_pac.close();
	cout << "local_cnip.pac生成成功！" << endl;
	cout << endl;
}
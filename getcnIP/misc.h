#pragma once
#include <sstream>

typedef long long ll;

ll string_to_num(const std::string& s);
std::string num_to_string(const ll& i);
std::string textfile2str(const std::string& PATH);
std::string& replace_all_distinct(std::string& str, const std::string& old_value, const std::string& new_value);
std::string NumberBaseConversion(const uint8_t& x, const uint8_t& y, const std::string& s);
extern const std::string pac_front_str;
extern const std::string pac_back_str1;
extern const std::string pac_back_str2;
extern const std::string user_dot_rule_front;
extern const std::string user_dot_rule_remote_proxy;
extern const std::string user_dot_rule_local_proxy;
extern const std::string user_dot_rule_direct;
extern const std::string user_dot_rule_reject;
extern const std::string user_dot_rule_host;
extern const std::string user_dot_rule_special;
extern const std::string user_dot_rule_ip;
extern const std::string user_dot_rule_local;
extern const std::string ss_cnip_front;
extern const std::string ss_cnip_cnIpRange1;
extern const std::string ss_cnip_cnIpRange2;
extern const std::string ss_cnip_cnIpRange3;
extern const std::string ss_cnip_cnIpRange4;
extern const std::string ss_cnip_cnIpRange5;
extern const std::string ss_cnip_cnIpRange6;
extern const std::string ss_cnip_cnIpRange7;
extern const std::string ss_cnip_cnIp16Range;
extern const std::string ss_cnip_whiteIpList;
extern const std::string ss_cnip_subnetIpRangeList;
extern const std::string ss_cnip_back;
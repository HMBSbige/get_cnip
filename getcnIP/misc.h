#pragma once
#include <sstream>

typedef long long ll;

ll string_to_num(const std::string& s);
std::string num_to_string(const ll& i);
std::string textfile2str(const std::string& PATH);
std::string& replace_all_distinct(std::string& str, const std::string& old_value, const std::string& new_value);
std::string NumberBaseConversion(const uint8_t& x, const uint8_t& y, const std::string& s);
std::pair<std::string, std::string> rsplit_blank(const std::string &str);
std::pair<std::string, std::string> rsplit_dot(const std::string &str);
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
extern const std::string ss_cnip_cnIpRange;
extern const std::string ss_cnip_cnIp16Range;
extern const std::string ss_cnip_whiteIpList;
extern const std::string ss_cnip_subnetIpRangeList;
extern const std::string ss_cnip_back;
extern const std::string ss_white_back;
extern const std::string ss_white_r_front;
extern const std::string proxy_pac_front;
extern const std::string whitelist_acl_front;
extern const std::string whitelist_acl_bypass_list;
extern const std::string whitelist_acl_local;
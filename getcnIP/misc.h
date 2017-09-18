#pragma once
#include <sstream>
#include <fstream>
typedef long long ll;
ll string_to_num(const std::string& s);
std::string num_to_string(const ll& i);
std::string textfile2str(const std::string& PATH);
std::string& replace_all_distinct(std::string& str, const std::string& old_value, const std::string& new_value);
extern const std::string pac_front_str;
extern const std::string pac_back_str1;
extern const std::string pac_back_str2;
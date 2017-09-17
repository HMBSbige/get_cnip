#include "misc.h"

ll string_to_num(const std::string& s) {
	ll num;
	std::stringstream ss(s);
	ss >> num;
	return num;
}
std::string num_to_string(const ll& i) {
	std::ostringstream oss;
	oss << i;
	return oss.str();
}
#include "ipv4.h"
ipv4::ipv4(const ipv4& a) :n_(4)
{
	n_[0] = a.n_[0];
	n_[1] = a.n_[1];
	n_[2] = a.n_[2];
	n_[3] = a.n_[3];
}
ipv4::ipv4(ll n1,ll n2,ll n3,ll n4) :n_(4)
{
	n_[0] = n1;
	n_[1] = n2;
	n_[2] = n3;
	n_[3] = n4;
}
ipv4::ipv4(std::string ip) :n_(4)
{
	get_ipv4(ip);
}
ipv4::ipv4(std::vector<ll> a)
{
	n_ = a;
}
void ipv4::get_ipv4(std::string ip) {
	auto pos1 = ip.find(".");
	n_[0] = string_to_num(std::string(ip, 0, pos1));
	auto pos2 = ip.find(".", pos1 + 1);
	n_[1] = string_to_num(std::string(ip, pos1 + 1, pos2 - pos1 - 1));
	pos1 = ip.find(".", pos2 + 1);
	n_[2] = string_to_num(std::string(ip, pos2 + 1, pos1 - pos2 - 1));
	pos2 = ip.size();
	n_[3] = string_to_num(std::string(ip, pos1 + 1, pos2 - pos1 - 1));
}
ll ipv4::get_ipv4(size_t i) {
	return this->n_.at(i);
}
std::vector<ll> ipv4::get_ipv4() const
{
	return n_;
}
std::string ipv4::str() {
	return num_to_string(n_[0]) + "." + num_to_string(n_[1]) + "." + num_to_string(n_[2]) + "." + num_to_string(n_[3]);
}

bool is_cn_ipv4(std::string line) {
	const std::string temp(line, 0, 14);
	if (temp == "apnic|CN|ipv4|")
		return true;
	return false;
}
std::string get_subnet_mask(ll num) {
	std::string mask[4];
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
	for (auto i = 0; i < 4; ++i)
		mask[i] = num_to_string(n[i]);
	return mask[0] + "." + mask[1] + "." + mask[2] + "." + mask[3];
}
std::string get_last_ip(std::string ip, ll num) {
	size_t i = 2;
	while (num / 256 > 256) {
		--i;
		num /= 256;
	}
	num /= 256;
	--num;
	auto n = ipv4(ip).get_ipv4();
	n[i] += num;
	for (++i; i < 4; ++i)
		n[i] = 255;
	return ipv4(n).str();
}
ip_list get_ip(std::string line) {
	std::string temp(line, 14, line.length() - 14);
	const auto pos1 = temp.find("|");
	const std::string ip(temp, 0, pos1);
	const auto pos2 = temp.find("|", pos1 + 1);
	const std::string number_of_ip(temp, pos1 + 1, pos2 - pos1 - 1);
	const auto mask = get_subnet_mask(string_to_num(number_of_ip));
	const auto last_ip = get_last_ip(ip, string_to_num(number_of_ip));
	return ip_list(ip, mask, last_ip);
}
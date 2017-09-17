#pragma once
#include "misc.h"
#include <vector>
typedef long long ll;
class ipv4 {
private:
	std::vector<ll> n_;
public:
	ipv4(const ipv4& a);
	explicit ipv4(ll n1 = 0, ll n2 = 0, ll n3 = 0, ll n4 = 0);
	explicit ipv4(std::string ip);
	explicit ipv4(std::vector<ll> a);
	void get_ipv4(std::string ip);
	ll get_ipv4(size_t i);
	std::vector<ll> get_ipv4() const;
	std::string str();
};
struct ip_list
{
	ipv4 first_ip;
	ipv4 last_ip;
	std::string mask, cn_ipv4;
	ip_list() {}
	ip_list(std::string a, std::string b, std::string c) :first_ip(a), last_ip(c), mask(b) {}

};
bool is_cn_ipv4(std::string line);
std::string get_subnet_mask(ll num);
std::string get_last_ip(std::string ip, ll num);
ip_list get_ip(std::string line);
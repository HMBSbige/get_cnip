#pragma once
#include "misc.h"
#include <vector>
typedef long long ll;
class IPv4 {
private:
	std::vector<ll> n_;
public:
	IPv4(const IPv4& a);
	explicit IPv4(ll n1 = 0, ll n2 = 0, ll n3 = 0, ll n4 = 0);
	explicit IPv4(std::string ip);
	explicit IPv4(std::vector<ll> a);
	void get_ipv4(std::string ip);
	ll get_ipv4(size_t i);
	std::vector<ll> get_ipv4() const;
	std::string str();
};
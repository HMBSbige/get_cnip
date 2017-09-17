#include "IPv4.h"
IPv4::IPv4(const IPv4& a) :n_(4)
{
	n_[0] = a.n_[0];
	n_[1] = a.n_[1];
	n_[2] = a.n_[2];
	n_[3] = a.n_[3];
}
IPv4::IPv4(ll n1,ll n2,ll n3,ll n4) :n_(4)
{
	n_[0] = n1;
	n_[1] = n2;
	n_[2] = n3;
	n_[3] = n4;
}
IPv4::IPv4(std::string ip) :n_(4)
{
	get_ipv4(ip);
}
IPv4::IPv4(std::vector<ll> a)
{
	n_ = a;
}
void IPv4::get_ipv4(std::string ip) {
	size_t pos1 = ip.find(".");
	n_[0] = string_to_num(std::string(ip, 0, pos1));
	size_t pos2 = ip.find(".", pos1 + 1);
	n_[1] = string_to_num(std::string(ip, pos1 + 1, pos2 - pos1 - 1));
	pos1 = ip.find(".", pos2 + 1);
	n_[2] = string_to_num(std::string(ip, pos2 + 1, pos1 - pos2 - 1));
	pos2 = ip.size();
	n_[3] = string_to_num(std::string(ip, pos1 + 1, pos2 - pos1 - 1));
}
ll IPv4::get_ipv4(size_t i) {
	return this->n_.at(i);
}

std::vector<ll> IPv4::get_ipv4() const
{
	return n_;
}

std::string IPv4::str() {
	return num_to_string(n_[0]) + "." + num_to_string(n_[1]) + "." + num_to_string(n_[2]) + "." + num_to_string(n_[3]);
}
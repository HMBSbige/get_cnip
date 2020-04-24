var direct = "__DIRECT__";
if (direct == "__DIR" + "ECT__") direct = "DIRECT;";

var wall_proxy = function(){ return "__PROXY__"; };
var wall_v6_proxy = function(){ return "__PROXY__"; };

var nowall_proxy = function(){ return direct; };
var ip_proxy = function(){ return wall_proxy(); };
var ipv6_proxy = function(){ return wall_v6_proxy(); };

var cnIpRange = __cnIpRange__
var cnIp16Range = __cnIp16Range__

var whiteIpList = {
	"188.188.188.188":1,
	"100.100.100.100":1
};

var subnetIpRangeList = [
0,16777216,             //0.0.0.0/8
167772160,184549376,    //10.0.0.0/8
1681915904,1686110208,  //100.64.0.0/10
2130706432,2147483648,  //127.0.0.0/8
2851995648,2852061184,  //169.254.0.0/16
2886729728,2887778304,  //172.16.0.0/12
3221225472,3221225728,  //192.0.0.0/24
3221225984,3221226240,  //192.0.2.0/24
3227017984,3227018240,  //192.88.99.0/24
3232235520,3232301056,  //192.168.0.0/16
3323068416,3323199488,  //198.18.0.0/15
3325256704,3325256960,  //198.51.100.0/24
3405803776,3405804032,  //203.0.113.0/24
3758096384,4026531840   //224.0.0.0/4
];

var white_domains = __white_domains__;

var hasOwnProperty = Object.hasOwnProperty;

function check_ipv4(host) {
	var re_ipv4 = /^\d+\.\d+\.\d+\.\d+$/g;
	if (re_ipv4.test(host)) {
		return true;
	}
}
function check_ipv6(host) {
	var re_ipv6 = /^\[?([a-fA-F0-9]{0,4}\:){1,7}[a-fA-F0-9]{0,4}\]?$/g;
	if (re_ipv6.test(host)) {
		return true;
	}
}
function check_ipv6_dns(dnsstr) {
	var re_ipv6 = /([a-fA-F0-9]{0,4}\:){1,7}[a-fA-F0-9]{0,4}(%[0-9]+)?/g;
	if (re_ipv6.test(dnsstr)) {
		return true;
	}
}
function convertAddress(ipchars) {
	var bytes = ipchars.split('.');
	var result = (bytes[0] << 24) |
	(bytes[1] << 16) |
	(bytes[2] << 8) |
	(bytes[3]);
	return result >>> 0;
}
function isInSingleRange(ipRange, intIp) {
	if ( hasOwnProperty.call(cnIp16Range, intIp >>> 6) ) {
		for ( var range = 1; range < 64; range*=4 ) {
			var master = intIp & ~(range-1);
			if ( hasOwnProperty.call(ipRange, master) )
				return intIp - master < ipRange[master];
		}
	} else {
		for ( var range = 64; range <= 1024; range*=4 ) {
			var master = intIp & ~(range-1);
			if ( hasOwnProperty.call(ipRange, master) )
				return intIp - master < ipRange[master];
		}
	}
}
function isInSubnetRange(ipRange, intIp) {
	for ( var i = 0; i < 28; i += 2 ) {
		if ( ipRange[i] <= intIp && intIp < ipRange[i+1] )
			return true;
	}
}
function getProxyFromIP(strIp) {
	var intIp = convertAddress(strIp);
	if ( isInSubnetRange(subnetIpRangeList, intIp) ) {
		return direct;
	}
	var index = (intIp >>> 24) & 0xff;
	if ( isInSingleRange(cnIpRange[index], intIp >>> 8) ) {
		return nowall_proxy();
	}
	if (hasOwnProperty.call(whiteIpList, strIp)) {
		return nowall_proxy();
	}
	return ip_proxy();
}
function isInDomains(domain_dict, host) {
	var suffix;
	var pos1 = host.lastIndexOf('.');

	suffix = host.substring(pos1 + 1);
	if (suffix == "cn") {
		return true;
	}

	var domains = domain_dict[suffix];
	if ( domains === undefined ) {
		return false;
	}
	host = host.substring(0, pos1);
	var pos = host.lastIndexOf('.');

	while(1) {
		if (pos <= 0) {
			if (hasOwnProperty.call(domains, host)) {
				return true;
			} else {
				return false;
			}
		}
		suffix = host.substring(pos + 1);
		if (hasOwnProperty.call(domains, suffix)) {
			return true;
		}
		pos = host.lastIndexOf('.', pos - 1);
	}
}
function FindProxyForURL(url, host) {
	if ( isPlainHostName(host) === true ) {
		return direct;
	}
	if ( check_ipv4(host) === true ) {
		return getProxyFromIP(host);
	}
	if ( check_ipv6(host) === true ) {
		return ipv6_proxy();
	}
	if ( isInDomains(white_domains, host) === true ) {
		return nowall_proxy();
	}
	return wall_proxy();
}

function FindProxyForURLEx(url, host) {
	if ( isPlainHostName(host) === true ) {
		return direct;
	}
	if ( check_ipv4(host) === true ) {
		return getProxyFromIP(host);
	}
	if ( check_ipv6(host) === true ) {
		return ipv6_proxy();
	}
	if ( isInDomains(white_domains, host) === true ) {
		return nowall_proxy();
	}
	return wall_proxy();
}
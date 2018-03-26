#include "misc.h"
#include <fstream>
#include <string>

ll string_to_num(const std::string& s) {
	return std::stoll(s);
}
std::string num_to_string(const ll& i) {
	return std::to_string(i);
}
std::string textfile2str(const std::string& PATH)
{
	std::ifstream file(PATH);
	std::ostringstream oss;
	oss << file.rdbuf();
	file.close();
	return oss.str();
}
std::string& replace_all_distinct(std::string& str, const std::string& old_value, const std::string& new_value)
{
	for (std::size_t pos(0); pos != std::string::npos; pos += new_value.length()) {
		if ((pos = str.find(old_value, pos)) != std::string::npos)
			str.replace(pos, old_value.length(), new_value);
		else
			break;
	}
	return str;
}
std::string NumberBaseConversion(const uint8_t& x, const uint8_t& y, const std::string& s) //2<=x,y<=36
{
	std::string res;
	ll sum = 0;
	if (x == 10)
		if (s[0] == '-')
			sum = stoll(s.substr(1));
		else
			sum = stoll(s);
	else
		for (int i = 0; i < s.size(); ++i) {
			if (s[i] >= '0' && s[i] <= '9')
				sum = sum*x + s[i] - '0';
			else if (s[i] >= 'A' && s[i] <= 'Z')
				sum = sum*x + s[i] - 'A' + 10;
			else if (s[i] >= 'a' && s[i] <= 'z')
				sum = sum*x + s[i] - 'a' + 10;
		}
	while (sum) {
		char tmp = sum%y;
		sum /= y;
		if (tmp <= 9)
			tmp += '0';
		else
			tmp -= 10 - 'a';
		res = tmp + res;
	}
	if (!res.size())
		return "0";
	if (s[0] == '-')
		return '-' + res;
	return res;
}
std::pair<std::string, std::string> rsplit_blank(const std::string &str)
{
	const auto pos = str.find(" ");
	if (pos == std::string::npos)
		return std::make_pair("", "");
	return make_pair(str.substr(0, pos), str.substr(pos + 1));
}
std::pair<std::string, std::string> rsplit_dot(const std::string &str)
{
	const auto pos = str.rfind(".");
	if (pos == std::string::npos)
		return std::make_pair("", "");
	return make_pair(str.substr(pos + 1), str.substr(0, pos));
}
extern const std::string pac_front_str = R"FUCK(var direct = "__DIRECT__";
if (direct == "__DIR" + "ECT__") direct = "DIRECT;";

var wall_proxy = function(){ return "__PROXY__"; };
var wall_v6_proxy = function(){ return "__PROXY__"; };

var nowall_proxy = function(){ return direct; };
var ip_proxy = function(){ return nowall_proxy(); };
var ipv6_proxy = function(){ return nowall_proxy(); };

var rules = [
)FUCK";
extern const std::string pac_back_str1 = R"FUCK(];

/*
* This file is part of Adblock Plus <http://adblockplus.org/>,
* Copyright (C) 2006-2014 Eyeo GmbH
*
* Adblock Plus is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 3 as
* published by the Free Software Foundation.
*
* Adblock Plus is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Adblock Plus.	If not, see <http://www.gnu.org/licenses/>.
*/

function createDict()
{
	var result = {};
	result.__proto__ = null;
	return result;
}

function getOwnPropertyDescriptor(obj, key)
{
	if (obj.hasOwnProperty(key))
	{
		return obj[key];
	}
	return null;
}

function extend(subclass, superclass, definition)
{
	if (Object.__proto__)
	{
		definition.__proto__ = superclass.prototype;
		subclass.prototype = definition;
	}
	else
	{
		var tmpclass = function(){}, ret;
		tmpclass.prototype = superclass.prototype;
		subclass.prototype = new tmpclass();
		subclass.prototype.constructor = superclass;
		for (var i in definition)
		{
			if (definition.hasOwnProperty(i))
			{
				subclass.prototype[i] = definition[i];
			}
		}
	}
}

function Filter(text)
{
	this.text = text;
	this.subscriptions = [];
}
Filter.prototype = {
	text: null,
	subscriptions: null,
	toString: function()
	{
		return this.text;
	}
};
Filter.knownFilters = createDict();
Filter.elemhideRegExp = /^([^\/\*\|\@"!]*?)#(\@)?(?:([\w\-]+|\*)((?:\([\w\-]+(?:[$^*]?=[^\(\)"]*)?\))*)|#([^{}]+))$/;
Filter.regexpRegExp = /^(@@)?\/.*\/(?:\$~?[\w\-]+(?:=[^,\s]+)?(?:,~?[\w\-]+(?:=[^,\s]+)?)*)?$/;
Filter.optionsRegExp = /\$(~?[\w\-]+(?:=[^,\s]+)?(?:,~?[\w\-]+(?:=[^,\s]+)?)*)$/;
Filter.fromText = function(text)
{
	if (text in Filter.knownFilters)
	{
		return Filter.knownFilters[text];
	}
	var ret;
	if (text.charAt(0) == "!")
	{
		ret = new CommentFilter(text);
	}
	else
	{
		ret = RegExpFilter.fromText(text);
	}
	Filter.knownFilters[ret.text] = ret;
	return ret;
};

function InvalidFilter(text, reason)
{
	Filter.call(this, text);
	this.reason = reason;
}
extend(InvalidFilter, Filter, {
	reason: null
});

function CommentFilter(text)
{
	Filter.call(this, text);
}
extend(CommentFilter, Filter, {
});

function ActiveFilter(text, domains)
{
	Filter.call(this, text);
	this.domainSource = domains;
}
extend(ActiveFilter, Filter, {
	domainSource: null,
	domainSeparator: null,
	ignoreTrailingDot: true,
	domainSourceIsUpperCase: false,
	getDomains: function()
	{
		var prop = getOwnPropertyDescriptor(this, "domains");
		if (prop)
		{
			return prop;
		}
		var domains = null;
		if (this.domainSource)
		{
			var source = this.domainSource;
			if (!this.domainSourceIsUpperCase)
			{
				source = source.toUpperCase();
			}
			var list = source.split(this.domainSeparator);
			if (list.length == 1 && (list[0]).charAt(0) != "~")
			{
				domains = createDict();
				domains[""] = false;
				if (this.ignoreTrailingDot)
				{
					list[0] = list[0].replace(/\.+$/, "");
				}
				domains[list[0]] = true;
			}
			else
			{
				var hasIncludes = false;
				for (var i = 0; i < list.length; i++)
				{
					var domain = list[i];
					if (this.ignoreTrailingDot)
					{
						domain = domain.replace(/\.+$/, "");
					}
					if (domain == "")
					{
						continue;
					}
					var include;
					if (domain.charAt(0) == "~")
					{
						include = false;
						domain = domain.substr(1);
					}
					else
					{
						include = true;
						hasIncludes = true;
					}
					if (!domains)
					{
						domains = createDict();
					}
					domains[domain] = include;
				}
				domains[""] = !hasIncludes;
			}
			this.domainSource = null;
		}
		return this.domains;
	},
	sitekeys: null,
	isActiveOnDomain: function(docDomain, sitekey)
	{
		if (this.getSitekeys() && (!sitekey || this.getSitekeys().indexOf(sitekey.toUpperCase()) < 0))
		{
			return false;
		}
		if (!this.getDomains())
		{
			return true;
		}
		if (!docDomain)
		{
			return this.getDomains()[""];
		}
		if (this.ignoreTrailingDot)
		{
			docDomain = docDomain.replace(/\.+$/, "");
		}
		docDomain = docDomain.toUpperCase();
		while (true)
		{
			if (docDomain in this.getDomains())
			{
				return this.domains[docDomain];
			}
			var nextDot = docDomain.indexOf(".");
			if (nextDot < 0)
			{
				break;
			}
			docDomain = docDomain.substr(nextDot + 1);
		}
		return this.domains[""];
	},
	isActiveOnlyOnDomain: function(docDomain)
	{
		if (!docDomain || !this.getDomains() || this.getDomains()[""])
		{
			return false;
		}
		if (this.ignoreTrailingDot)
		{
			docDomain = docDomain.replace(/\.+$/, "");
		}
		docDomain = docDomain.toUpperCase();
		for (var domain in this.getDomains())
		{
			if (this.domains[domain] && domain != docDomain && (domain.length <= docDomain.length || domain.indexOf("." + docDomain) != domain.length - docDomain.length - 1))
			{
				return false;
			}
		}
		return true;
	}
});

function RegExpFilter(text, regexpSource, contentType, matchCase, domains, thirdParty, sitekeys)
{
	ActiveFilter.call(this, text, domains, sitekeys);
	if (contentType != null)
	{
		this.contentType = contentType;
	}
	if (matchCase)
	{
		this.matchCase = matchCase;
	}
	if (thirdParty != null)
	{
		this.thirdParty = thirdParty;
	}
	if (sitekeys != null)
	{
		this.sitekeySource = sitekeys;
	}
	if (regexpSource.length >= 2 && regexpSource.charAt(0) == "/" && regexpSource.charAt(regexpSource.length - 1) == "/")
	{
		var regexp = new RegExp(regexpSource.substr(1, regexpSource.length - 2), this.matchCase ? "" : "i");
		this.regexp = regexp;
	}
	else
	{
		this.regexpSource = regexpSource;
	}
}
extend(RegExpFilter, ActiveFilter, {
	domainSourceIsUpperCase: true,
	length: 1,
	domainSeparator: "|",
	regexpSource: null,
	getRegexp: function()
	{
		var prop = getOwnPropertyDescriptor(this, "regexp");
		if (prop)
		{
			return prop;
		}
		var source = this.regexpSource.replace(/\*+/g, "*").replace(/\^\|$/, "^").replace(/\W/g, "\\$&").replace(/\\\*/g, ".*").replace(/\\\^/g, "(?:[\\x00-\\x24\\x26-\\x2C\\x2F\\x3A-\\x40\\x5B-\\x5E\\x60\\x7B-\\x7F]|$)").replace(/^\\\|\\\|/, "^[\\w\\-]+:\\/+(?!\\/)(?:[^\\/]+\\.)?").replace(/^\\\|/, "^").replace(/\\\|$/, "$").replace(/^(\.\*)/, "").replace(/(\.\*)$/, "");
		var regexp = new RegExp(source, this.matchCase ? "" : "i");
		this.regexp = regexp;
		return regexp;
	},
	contentType: 2147483647,
	matchCase: false,
	thirdParty: null,
	sitekeySource: null,
	getSitekeys: function()
	{
		var prop = getOwnPropertyDescriptor(this, "sitekeys");
		if (prop)
		{
			return prop;
		}
		var sitekeys = null;
		if (this.sitekeySource)
		{
			sitekeys = this.sitekeySource.split("|");
			this.sitekeySource = null;
		}
		this.sitekeys = sitekeys;
		return this.sitekeys;
	},
	matches: function(location, contentType, docDomain, thirdParty, sitekey)
	{
		if (this.getRegexp().test(location) && this.isActiveOnDomain(docDomain, sitekey))
		{
			return true;
		}
		return false;
	}
});
RegExpFilter.prototype["0"] = "#this";
RegExpFilter.fromText = function(text)
{
	var blocking = true;
	var origText = text;
	if (text.indexOf("@@") == 0)
	{
		blocking = false;
		text = text.substr(2);
	}
	var contentType = null;
	var matchCase = null;
	var domains = null;
	var sitekeys = null;
	var thirdParty = null;
	var collapse = null;
	var options;
	var match = text.indexOf("$") >= 0 ? Filter.optionsRegExp.exec(text) : null;
	if (match)
	{
		options = match[1].toUpperCase().split(",");
		text = match.input.substr(0, match.index);
		for (var _loopIndex6 = 0; _loopIndex6 < options.length; ++_loopIndex6)
		{
			var option = options[_loopIndex6];
			var value = null;
			var separatorIndex = option.indexOf("=");
			if (separatorIndex >= 0)
			{
				value = option.substr(separatorIndex + 1);
				option = option.substr(0, separatorIndex);
			}
			option = option.replace(/-/, "_");
			if (option in RegExpFilter.typeMap)
			{
				if (contentType == null)
				{
					contentType = 0;
				}
				contentType |= RegExpFilter.typeMap[option];
			}
			else if (option.charAt(0) == "~" && option.substr(1) in RegExpFilter.typeMap)
			{
				if (contentType == null)
				{
					contentType = RegExpFilter.prototype.contentType;
				}
				contentType &= ~RegExpFilter.typeMap[option.substr(1)];
			}
			else if (option == "MATCH_CASE")
			{
				matchCase = true;
			}
			else if (option == "~MATCH_CASE")
			{
				matchCase = false;
			}
			else if (option == "DOMAIN" && typeof value != "undefined")
			{
				domains = value;
			}
			else if (option == "THIRD_PARTY")
			{
				thirdParty = true;
			}
			else if (option == "~THIRD_PARTY")
			{
				thirdParty = false;
			}
			else if (option == "COLLAPSE")
			{
				collapse = true;
			}
			else if (option == "~COLLAPSE")
			{
				collapse = false;
			}
			else if (option == "SITEKEY" && typeof value != "undefined")
			{
				sitekeys = value;
			}
			else
			{
				return new InvalidFilter(origText, "Unknown option " + option.toLowerCase());
			}
		}
	}
	if (!blocking && (contentType == null || contentType & RegExpFilter.typeMap.DOCUMENT) && (!options || options.indexOf("DOCUMENT") < 0) && !/^\|?[\w\-]+:/.test(text))
	{
		if (contentType == null)
		{
			contentType = RegExpFilter.prototype.contentType;
		}
		contentType &= ~RegExpFilter.typeMap.DOCUMENT;
	}
	try
	{
		if (blocking)
		{
			return new BlockingFilter(origText, text, contentType, matchCase, domains, thirdParty, sitekeys, collapse);
		}
		else
		{
			return new WhitelistFilter(origText, text, contentType, matchCase, domains, thirdParty, sitekeys);
		}
	}
	catch (e)
	{
		return new InvalidFilter(origText, e);
	}
};
RegExpFilter.typeMap = {
	OTHER: 1,
	SCRIPT: 2,
	IMAGE: 4,
	STYLESHEET: 8,
	OBJECT: 16,
	SUBDOCUMENT: 32,
	DOCUMENT: 64,
	XBL: 1,
	PING: 1,
	XMLHTTPREQUEST: 2048,
	OBJECT_SUBREQUEST: 4096,
	DTD: 1,
	MEDIA: 16384,
	FONT: 32768,
	BACKGROUND: 4,
	POPUP: 268435456,
	ELEMHIDE: 1073741824
};
RegExpFilter.prototype.contentType &= ~ (RegExpFilter.typeMap.ELEMHIDE | RegExpFilter.typeMap.POPUP);

function BlockingFilter(text, regexpSource, contentType, matchCase, domains, thirdParty, sitekeys, collapse)
{
	RegExpFilter.call(this, text, regexpSource, contentType, matchCase, domains, thirdParty, sitekeys);
	this.collapse = collapse;
}
extend(BlockingFilter, RegExpFilter, {
	collapse: null
});

function WhitelistFilter(text, regexpSource, contentType, matchCase, domains, thirdParty, sitekeys)
{
	RegExpFilter.call(this, text, regexpSource, contentType, matchCase, domains, thirdParty, sitekeys);
}
extend(WhitelistFilter, RegExpFilter, {
});

)FUCK";
extern const std::string pac_back_str2 = R"FUCK(function Matcher()
{
	this.clear();
}
Matcher.prototype = {
	filterByKeyword: null,
	keywordByFilter: null,
	clear: function()
	{
		this.filterByKeyword = createDict();
		this.keywordByFilter = createDict();
	},
	add: function(filter)
	{
		if (filter.text in this.keywordByFilter)
		{
			return;
		}
		var keyword = this.findKeyword(filter);
		var oldEntry = this.filterByKeyword[keyword];
		if (typeof oldEntry == "undefined")
		{
			this.filterByKeyword[keyword] = filter;
		}
		else if (oldEntry.length == 1)
		{
			this.filterByKeyword[keyword] = [oldEntry, filter];
		}
		else
		{
			oldEntry.push(filter);
		}
		this.keywordByFilter[filter.text] = keyword;
	},
	remove: function(filter)
	{
		if (!(filter.text in this.keywordByFilter))
		{
			return;
		}
		var keyword = this.keywordByFilter[filter.text];
		var list = this.filterByKeyword[keyword];
		if (list.length <= 1)
		{
			delete this.filterByKeyword[keyword];
		}
		else
		{
			var index = list.indexOf(filter);
			if (index >= 0)
			{
				list.splice(index, 1);
				if (list.length == 1)
				{
					this.filterByKeyword[keyword] = list[0];
				}
			}
		}
		delete this.keywordByFilter[filter.text];
	},
	findKeyword: function(filter)
	{
		var result = "";
		var text = filter.text;
		if (Filter.regexpRegExp.test(text))
		{
			return result;
		}
		var match = Filter.optionsRegExp.exec(text);
		if (match)
		{
			text = match.input.substr(0, match.index);
		}
		if (text.substr(0, 2) == "@@")
		{
			text = text.substr(2);
		}
		var candidates = text.toLowerCase().match(/[^a-z0-9%*][a-z0-9%]{3,}(?=[^a-z0-9%*])/g);
		if (!candidates)
		{
			return result;
		}
		var hash = this.filterByKeyword;
		var resultCount = 16777215;
		var resultLength = 0;
		for (var i = 0, l = candidates.length; i < l; i++)
		{
			var candidate = candidates[i].substr(1);
			var count = candidate in hash ? hash[candidate].length : 0;
			if (count < resultCount || count == resultCount && candidate.length > resultLength)
			{
				result = candidate;
				resultCount = count;
				resultLength = candidate.length;
			}
		}
		return result;
	},
	hasFilter: function(filter)
	{
		return filter.text in this.keywordByFilter;
	},
	getKeywordForFilter: function(filter)
	{
		if (filter.text in this.keywordByFilter)
		{
			return this.keywordByFilter[filter.text];
		}
		else
		{
			return null;
		}
	},
	_checkEntryMatch: function(keyword, location, contentType, docDomain, thirdParty, sitekey)
	{
		var list = this.filterByKeyword[keyword];
		for (var i = 0; i < list.length; i++)
		{
			var filter = list[i];
			if (filter == "#this")
			{
				filter = list;
			}
			if (filter.matches(location, contentType, docDomain, thirdParty, sitekey))
			{
				return filter;
			}
		}
		return null;
	},
	matchesAny: function(location, contentType, docDomain, thirdParty, sitekey)
	{
		var candidates = location.toLowerCase().match(/[a-z0-9%]{3,}/g);
		if (candidates === null)
		{
			candidates = [];
		}
		candidates.push("");
		for (var i = 0, l = candidates.length; i < l; i++)
		{
			var substr = candidates[i];
			if (substr in this.filterByKeyword)
			{
				var result = this._checkEntryMatch(substr, location, contentType, docDomain, thirdParty, sitekey);
				if (result)
				{
					return result;
				}
			}
		}
		return null;
	}
};

function CombinedMatcher()
{
	this.blacklist = new Matcher();
	this.whitelist = new Matcher();
	this.resultCache = createDict();
}
CombinedMatcher.maxCacheEntries = 1000;
CombinedMatcher.prototype = {
	blacklist: null,
	whitelist: null,
	resultCache: null,
	cacheEntries: 0,
	clear: function()
	{
		this.blacklist.clear();
		this.whitelist.clear();
		this.resultCache = createDict();
		this.cacheEntries = 0;
	},
	add: function(filter)
	{
		if (filter instanceof WhitelistFilter)
		{
			this.whitelist.add(filter);
		}
		else
		{
			this.blacklist.add(filter);
		}
		if (this.cacheEntries > 0)
		{
			this.resultCache = createDict();
			this.cacheEntries = 0;
		}
	},
	remove: function(filter)
	{
		if (filter instanceof WhitelistFilter)
		{
			this.whitelist.remove(filter);
		}
		else
		{
			this.blacklist.remove(filter);
		}
		if (this.cacheEntries > 0)
		{
			this.resultCache = createDict();
			this.cacheEntries = 0;
		}
	},
	findKeyword: function(filter)
	{
		if (filter instanceof WhitelistFilter)
		{
			return this.whitelist.findKeyword(filter);
		}
		else
		{
			return this.blacklist.findKeyword(filter);
		}
	},
	hasFilter: function(filter)
	{
		if (filter instanceof WhitelistFilter)
		{
			return this.whitelist.hasFilter(filter);
		}
		else
		{
			return this.blacklist.hasFilter(filter);
		}
	},
	getKeywordForFilter: function(filter)
	{
		if (filter instanceof WhitelistFilter)
		{
			return this.whitelist.getKeywordForFilter(filter);
		}
		else
		{
			return this.blacklist.getKeywordForFilter(filter);
		}
	},
	isSlowFilter: function(filter)
	{
		var matcher = filter instanceof WhitelistFilter ? this.whitelist : this.blacklist;
		if (matcher.hasFilter(filter))
		{
			return !matcher.getKeywordForFilter(filter);
		}
		else
		{
			return !matcher.findKeyword(filter);
		}
	},
	matchesAnyInternal: function(location, contentType, docDomain, thirdParty, sitekey)
	{
		var candidates = location.toLowerCase().match(/[a-z0-9%]{3,}/g);
		if (candidates === null)
		{
			candidates = [];
		}
		candidates.push("");
		var blacklistHit = null;
		for (var i = 0, l = candidates.length; i < l; i++)
		{
			var substr = candidates[i];
			if (substr in this.whitelist.filterByKeyword)
			{
				var result = this.whitelist._checkEntryMatch(substr, location, contentType, docDomain, thirdParty, sitekey);
				if (result)
				{
					return result;
				}
			}
			if (substr in this.blacklist.filterByKeyword && blacklistHit === null)
			{
				blacklistHit = this.blacklist._checkEntryMatch(substr, location, contentType, docDomain, thirdParty, sitekey);
			}
		}
		return blacklistHit;
	},
	matchesAny: function(location, docDomain)
	{
		var key = location + " " + docDomain + " ";
		if (key in this.resultCache)
		{
			return this.resultCache[key];
		}
		var result = this.matchesAnyInternal(location, 0, docDomain, null, null);
		if (this.cacheEntries >= CombinedMatcher.maxCacheEntries)
		{
			this.resultCache = createDict();
			this.cacheEntries = 0;
		}
		this.resultCache[key] = result;
		this.cacheEntries++;
		return result;
	}
};
var defaultMatcher = new CombinedMatcher();

for (var i = 0; i < rules.length; i++) {
	defaultMatcher.add(Filter.fromText(rules[i]));
}

var subnetIpRangeList = [
0,16777216,				//0.0.0.0/8
167772160,184549376,	//10.0.0.0/8
1681915904,1686110208,	//100.64.0.0/10
2130706432,2147483648,	//127.0.0.0/8
2851995648,2852061184,	//169.254.0.0/16
2886729728,2887778304,	//172.16.0.0/12
3221225472,3221225728,	//192.0.0.0/24
3221225984,3221226240,	//192.0.2.0/24
3227017984,3227018240,	//192.88.99.0/24
3232235520,3232301056,	//192.168.0.0/16
3323068416,3323199488,	//198.18.0.0/15
3325256704,3325256960,	//198.51.100.0/24
3405803776,3405804032,	//203.0.113.0/24
3758096384,4026531840	//224.0.0.0/4
];

function convertAddress(ipchars) {
	var bytes = ipchars.split('.');
	var result = (bytes[0] << 24) |
	(bytes[1] << 16) |
	(bytes[2] << 8) |
	(bytes[3]);
	return result >>> 0;
}

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
	return ip_proxy();
}

function FindProxyForURL(url, host) {
	if ( isPlainHostName(host) === true ) {
		return direct;
	}
	if (defaultMatcher.matchesAny(url, host) instanceof BlockingFilter) {
		return wall_proxy();
	}
	if ( check_ipv4(host) === true ) {
		return getProxyFromIP(host);
	}
	return direct;
})FUCK";
extern const std::string user_dot_rule_front = R"wtf(# A line which start with "#" is comment
# The new one will overwrite the same item before
# "a.com" means "a.com" only, "*.a.com" means all sub domains of "a.com" only, ".a.com" means "*.a.com" & "a.com"

)wtf";
extern const std::string user_dot_rule_remote_proxy = R"wtf(# remote proxy: transfer with server

)wtf";
extern const std::string user_dot_rule_local_proxy = R"wtf(# local proxy: direct connect or go through local proxy

)wtf";
extern const std::string user_dot_rule_direct =R"wtf(# direct

)wtf";
extern const std::string user_dot_rule_reject = R"wtf(# reject: reject this host

)wtf";
extern const std::string user_dot_rule_host = R"wtf(# host list

)wtf";
extern const std::string user_dot_rule_special = R"wtf(# special rule

)wtf";
extern const std::string user_dot_rule_ip = R"wtf(# ip rules, run this rules if not match any host rule

:: ffff:ffff:ffff:ffff:ffff:ffff:ffff:ffff remoteproxy
0.0.0.0 255.255.255.255 remoteproxy

)wtf";
extern const std::string user_dot_rule_local = R"wtf(# local
10.0.0.0 10.255.255.255 direct
172.16.0.0 172.31.255.255 direct
192.168.0.0 192.168.255.255 direct
169.254.0.0 169.254.255.255 direct
127.0.0.0 127.255.255.255 direct
)wtf";
extern const std::string ss_cnip_front = R"wtf(var direct = "__DIRECT__";
if (direct == "__DIR" + "ECT__") direct = "DIRECT;";

var wall_proxy = function(){ return "__PROXY__"; };
var wall_v6_proxy = function(){ return "__PROXY__"; };

var nowall_proxy = function(){ return direct; };
var ip_proxy = function(){ return wall_proxy(); };
var ipv6_proxy = function(){ return wall_v6_proxy(); };

)wtf";

extern const std::string ss_cnip_cnIpRange = R"wtf(var cnIpRange = )wtf";

extern const std::string ss_cnip_cnIp16Range = R"wtf(var cnIp16Range = )wtf";
extern const std::string ss_cnip_whiteIpList = R"wtf(var whiteIpList = {
	"188.188.188.188":1,
	"100.100.100.100":1
};

)wtf";
extern const std::string ss_cnip_subnetIpRangeList = R"wtf(var subnetIpRangeList = [
0,16777216,				//0.0.0.0/8
167772160,184549376,	//10.0.0.0/8
1681915904,1686110208,	//100.64.0.0/10
2130706432,2147483648,	//127.0.0.0/8
2851995648,2852061184,	//169.254.0.0/16
2886729728,2887778304,	//172.16.0.0/12
3221225472,3221225728,	//192.0.0.0/24
3221225984,3221226240,	//192.0.2.0/24
3227017984,3227018240,	//192.88.99.0/24
3232235520,3232301056,	//192.168.0.0/16
3323068416,3323199488,	//198.18.0.0/15
3325256704,3325256960,	//198.51.100.0/24
3405803776,3405804032,	//203.0.113.0/24
3758096384,4026531840	//224.0.0.0/4
];

)wtf";
extern const std::string ss_cnip_back=R"wtf(var hasOwnProperty = Object.hasOwnProperty;

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
	if ( host.indexOf('steamcommunity.com') != -1 || host.indexOf('steamstatic.com/') != -1 || host.indexOf('steamcommunity-a.akamaihd.net') != -1 || host.indexOf('steamuserimages-a.akamaihd.net') != -1 || host.indexOf('steamstore-a.akamaihd.net') != -1) {
		return "PROXY steam.xunyou.com:15000";
	}
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

	var strIp = dnsResolve(host);
	if ( !strIp ) {
		return wall_proxy();
	}
	
	return getProxyFromIP(strIp);
}

function FindProxyForURLEx(url, host) {
	if ( host.indexOf('steamcommunity.com') != -1 || host.indexOf('steamstatic.com/') != -1 || host.indexOf('steamcommunity-a.akamaihd.net') != -1 || host.indexOf('steamuserimages-a.akamaihd.net') != -1 || host.indexOf('steamstore-a.akamaihd.net') != -1) {
		return "PROXY steam.xunyou.com:15000";
	}
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

	var strIp = dnsResolveEx(host);
	if ( !strIp ) {
		return wall_proxy();
	}
	if ( check_ipv6_dns(strIp) === true ) {
		return ipv6_proxy();
	}
	var dnsIps = strIp.split(";");
	if (check_ipv4(dnsIps[0]) === true) {
		return getProxyFromIP(dnsIps[0]);
	} else if (check_ipv6_dns(dnsIps[0]) === true) {
		return ipv6_proxy();
	}
	return wall_proxy();
})wtf";
extern const std::string ss_white_back = R"wtf(var hasOwnProperty = Object.hasOwnProperty;

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
	if ( host.indexOf('steamcommunity.com') != -1 || host.indexOf('steamstatic.com/') != -1 || host.indexOf('steamcommunity-a.akamaihd.net') != -1 || host.indexOf('steamuserimages-a.akamaihd.net') != -1 || host.indexOf('steamstore-a.akamaihd.net') != -1) {
		return "PROXY steam.xunyou.com:15000";
	}
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
	if ( host.indexOf('steamcommunity.com') != -1 || host.indexOf('steamstatic.com/') != -1 || host.indexOf('steamcommunity-a.akamaihd.net') != -1 || host.indexOf('steamuserimages-a.akamaihd.net') != -1 || host.indexOf('steamstore-a.akamaihd.net') != -1) {
		return "PROXY steam.xunyou.com:15000";
	}
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
})wtf";
extern const std::string ss_white_r_front = R"wtf(var direct = "__DIRECT__";
if (direct == "__DIR" + "ECT__") direct = "DIRECT;";

var wall_proxy = function(){ return direct; };
var wall_v6_proxy = function(){ return direct; };

var nowall_proxy = function(){ return "__PROXY__"; };
var ip_proxy = function(){ return nowall_proxy(); };
var ipv6_proxy = function(){ return wall_v6_proxy(); };

)wtf";
extern const std::string proxy_pac_front = R"wtf(var direct = "DIRECT;";
if (direct == "__DIR" + "ECT__") direct = "DIRECT;";

var wall_proxy = function(){ return "PROXY 127.0.0.1:23333;DIRECT;"; };
var wall_v6_proxy = function(){ return "PROXY 127.0.0.1:23333;DIRECT;"; };

var nowall_proxy = function(){ return direct; };
var ip_proxy = function(){ return wall_proxy(); };
var ipv6_proxy = function(){ return wall_v6_proxy(); };

)wtf";
extern const std::string whitelist_acl_front = u8R"wtf(#**********************************************************************
# [proxy_all] 默认代理
# [bypass_all] 默认直连
# [outbound_block_list] 禁止访问列表
# [bypass_list] 直连列表
# [proxy_list] 代理列表
# [remote_dns] 远程 DNS 解析 -不加使用本地 DNS
#**********************************************************************
[proxy_all]

)wtf";
extern const std::string whitelist_acl_bypass_list = u8R"wtf([bypass_list]

# CN域名直连
^(.*)\.cn$

# 中国国内常见域名关键词直连
^cn\.\w(.*)$
(^|\.)\w*-cn\w*\.\w*$
# ^.*steam.*$

# accelerated-domains china
)wtf";
extern const std::string whitelist_acl_local = u8R"wtf(#**********************************************************************
# 本地/局域网地址
^(.*\.)?local$
^(.*\.)?localhost$
^(.*\.)?ip6-localhost$
^(.*\.)?ip6-loopback$

# https://zh.wikipedia.org/wiki/%E4%BF%9D%E7%95%99IP%E5%9C%B0%E5%9D%80
0.0.0.0/8
10.0.0.0/8
100.64.0.0/10
127.0.0.0/8
169.254.0.0/16
172.16.0.0/12
192.0.0.0/24
192.0.2.0/24
192.88.99.0/24
192.168.0.0/16
198.18.0.0/15
198.51.100.0/24
203.0.113.0/24
224.0.0.0/4

# 国内ip地址
)wtf";
// = R"wtf()wtf";
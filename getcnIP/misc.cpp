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
0,1,
167772160,184549376,	//10.0.0.0/8
2886729728,2887778304,	//172.16.0.0/12
3232235520,3232301056,	//192.168.0.0/16
2130706432,2130706688	//127.0.0.0/24
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
	for ( var i = 0; i < 10; i += 2 ) {
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

extern const std::string ss_cnip_cnIp16Range = R"wtf(var cnIp16Range = {
0x400:1,0x404:1,0x408:1,0x410:1,0x428:1,0x3800:1,0x3804:1,0x3805:1,0x399a:1,0x3b00:1,0x3b01:1,0x6c02:1,0x6c03:1,0x6c8b:1,0x6cc8:1,0x6cd9:1,0x6cda:1,0x6d8b:1,0x6dab:1,0x6db4:1,0x6db5:1,0x6dc1:1,0x6dd0:1,0x6de5:1,0x6e0f:1,0x9000:1,0x9094:1,0x93fd:1,0x93fe:1,0x93ff:1,0x9c00:1,0xa800:1,0xa804:1,0xa8fa:1,0xa94d:1,0xa981:1,0xa98d:1,0xa9ec:1,0xaa70:1,0xaaed:1,0xab08:1,0xaf80:1,0xaf81:1,0xaf82:1,0xaf83:1,0xaf85:1,0xaf86:1,0xaf87:1,0xaf88:1,0xaf89:1,0xaf8a:1,0xaf8b:1,0xaf8c:1,0xaf8d:1,0xaf8e:1,0xaf8f:1,0xaf90:1,0xaf91:1,0xaf92:1,0xaf93:1,0xaf94:1,0xaf95:1,0xaf96:1,0xaf97:1,0xaf98:1,0xaf99:1,0xaf9a:1,0xaf9b:1,0xaf9c:1,0xaf9d:1,0xaf9e:1,0xafb0:1,0xafb1:1,0xafb2:1,0xafb3:1,0xafb4:1,0xafb5:1,0xafb6:1,0xafb7:1,0xafb8:1,0xafb9:1,0xafba:1,0xafbb:1,0xafbc:1,0xafbd:1,0xafbe:1,0xafc0:1,0xafc1:1,0xafc2:1,0xafc3:1,0xafc4:1,0xafc5:1,0xafc6:1,0xafc7:1,0xafc8:1,0xafc9:1,0xafca:1,0xafcb:1,0xafcc:1,0xafcd:1,0xafce:1,0xafcf:1,0xafd8:1,0xafd9:1,0xafdb:1,0xafdc:1,0xafdd:1,0xafde:1,0xafdf:1,0xafe0:1,0xafe1:1,0xafe2:1,0xafe3:1,0xafe4:1,0xafe5:1,0xafe6:1,0xafe7:1,0xafe8:1,0xafe9:1,0xafea:1,0xafeb:1,0xafec:1,0xafed:1,0xafef:1,0xaff0:1,0xaff3:1,0xaff8:1,0xaff9:1,0xaffa:1,0xaffb:1,0xaffc:1,0xaffd:1,0xaffe:1,0xafff:1,0xb504:1,0xb5c2:1,0xb5c3:1,0xb5c4:1,0xb5c5:1,0xb5c6:1,0xb5c7:1,0xb5c8:1,0xb5c9:1,0xb5ca:1,0xb5cb:1,0xb5cc:1,0xb5cd:1,0xb5ce:1,0xb5cf:1,0xb5d0:1,0xb5d1:1,0xb5d2:1,0xb5d3:1,0xb5d4:1,0xb5d5:1,0xb5d7:1,0xb5dc:1,0xb5dd:1,0xb5de:1,0xb5df:1,0xb5e1:1,0xb5e2:1,0xb5e3:1,0xb5e4:1,0xb5e5:1,0xb5e6:1,0xb5e7:1,0xb5e8:1,0xb5e9:1,0xb5ea:1,0xb5eb:1,0xb5ec:1,0xb5ed:1,0xb5ee:1,0xb5ef:1,0xb5f0:1,0xb5f1:1,0xb5f2:1,0xb5f3:1,0xb5f4:1,0xb5f5:1,0xb5f6:1,0xb5f8:1,0xb5f9:1,0xb5fb:1,0xb5fc:1,0xb5fd:1,0xb5fe:1,0xb5ff:1,0xc600:1,0xc7db:1,0xe907:1,0xee60:1,0xee61:1,0xee64:1,0xee65:1,0xee66:1,0xee67:1,0xeeff:1,0xf411:1,0xf412:1,0xf422:1,0xf43b:1,0xf470:1,0xf477:1,0xf4b7:1,0xf619:1,0x19400:1,0x19404:1,0x1940a:1,0x194c8:1,0x194d5:1,0x194df:1,0x19538:1,0x19580:1,0x1958d:1,0x19595:1,0x19599:1,0x195b9:1,0x19600:1,0x1972e:1,0x197a9:1,0x197ec:1,0x19c04:1,0x19c05:1,0x19c06:1,0x19c09:1,0x19c0a:1,0x19c0b:1,0x19c0d:1,0x19c0e:1,0x19c10:1,0x19c12:1,0x19c13:1,0x19c14:1,0x19c16:1,0x19c17:1,0x19c19:1,0x19c1b:1,0x19c1c:1,0x19c1e:1,0x19c1f:1,0x19c20:1,0x19c21:1,0x19c22:1,0x19c23:1,0x19c24:1,0x19c25:1,0x19c26:1,0x19c27:1,0x19c28:1,0x19c29:1,0x19c2a:1,0x19c2e:1,0x19c30:1,0x19c31:1,0x19c32:1,0x19c33:1,0x19c34:1,0x19c35:1,0x19c36:1,0x19c37:1,0x19c39:1,0x19c3a:1,0x19c3b:1,0x19c3c:1,0x19c3d:1,0x19c3f:1,0x19c40:1,0x19c41:1,0x19c44:1,0x19c45:1,0x19c46:1,0x19c47:1,0x19c4b:1,0x19c4c:1,0x19c4d:1,0x19c4f:1,0x19c50:1,0x19c51:1,0x19c52:1,0x19c53:1,0x19c55:1,0x19c56:1,0x19c57:1,0x19c58:1,0x19c59:1,0x19c5a:1,0x19c5b:1,0x19c5c:1,0x19c5e:1,0x19c5f:1,0x19c61:1,0x19c62:1,0x19c63:1,0x19c64:1,0x19c65:1,0x19c66:1,0x19c67:1,0x19c68:1,0x19c69:1,0x19c6a:1,0x19c6b:1,0x19c6c:1,0x19c6d:1,0x19c6f:1,0x19c70:1,0x19c73:1,0x19c74:1,0x19c76:1,0x19c78:1,0x19c79:1,0x19c7a:1,0x19c7b:1,0x19c7c:1,0x19c7d:1,0x19c7e:1,0x19c7f:1,0x19c80:1,0x19c81:1,0x19c82:1,0x19c83:1,0x19c84:1,0x19c85:1,0x19c86:1,0x19c87:1,0x19c88:1,0x19c89:1,0x19c8a:1,0x19c8b:1,0x19c8c:1,0x19c8d:1,0x19c8f:1,0x19c90:1,0x19c91:1,0x19c92:1,0x19c93:1,0x19c94:1,0x19c95:1,0x19c96:1,0x19c97:1,0x19c98:1,0x19c99:1,0x19c9a:1,0x19c9b:1,0x19c9c:1,0x19c9d:1,0x19c9e:1,0x19c9f:1,0x19ca0:1,0x19ca1:1,0x19ca3:1,0x19ca4:1,0x19ca5:1,0x19ca6:1,0x19ca7:1,0x19ca8:1,0x19ca9:1,0x19caa:1,0x19cab:1,0x19cac:1,0x19cad:1,0x19cae:1,0x19caf:1,0x19cb0:1,0x19cb1:1,0x19cb2:1,0x19cb3:1,0x19cb4:1,0x19cb5:1,0x19cb6:1,0x19cb7:1,0x19cb8:1,0x19cb9:1,0x19cba:1,0x19cbb:1,0x19cbc:1,0x19cbd:1,0x19cbe:1,0x19cbf:1,0x19cc0:1,0x19cc1:1,0x19cc2:1,0x19cc3:1,0x19cc4:1,0x19cc5:1,0x19cc6:1,0x19cc7:1,0x19cc8:1,0x19cc9:1,0x19cca:1,0x19ccb:1,0x19cd0:1,0x19cd1:1,0x19cd2:1,0x19cd3:1,0x19cd4:1,0x19cd5:1,0x19cd6:1,0x19cd7:1,0x19cd8:1,0x19cda:1,0x19cdb:1,0x19cdc:1,0x19cdd:1,0x19cde:1,0x19cdf:1,0x19ce0:1,0x19ce1:1,0x19ce2:1,0x19ce3:1,0x19ce4:1,0x19ce5:1,0x19ce6:1,0x19ce7:1,0x19ce8:1,0x19cea:1,0x19ced:1,0x19cee:1,0x19cef:1,0x19cf0:1,0x19cf2:1,0x19cf3:1,0x19cf4:1,0x19cf5:1,0x19cf6:1,0x19cf8:1,0x19cf9:1,0x19cfa:1,0x19cfb:1,0x19cfc:1,0x19cfd:1,0x19cfe:1,0x19cff:1,0x19f00:1,0x19f01:1,0x19f02:1,0x19f03:1,0x19f04:1,0x19f05:1,0x19f06:1,0x19f07:1,0x19f08:1,0x19f0b:1,0x19f0d:1,0x19f0e:1,0x19f0f:1,0x19f10:1,0x19f11:1,0x19f12:1,0x19f13:1,0x19f16:1,0x19f17:1,0x19f18:1,0x19f19:1,0x19f1a:1,0x19f1b:1,0x19f1e:1,0x19f1f:1,0x19f20:1,0x19f21:1,0x19f22:1,0x19f23:1,0x19f24:1,0x19f25:1,0x19f26:1,0x19f27:1,0x19f28:1,0x19f29:1,0x19f2a:1,0x19f2b:1,0x19f2c:1,0x19f2d:1,0x19f2e:1,0x19f2f:1,0x19f30:1,0x19f31:1,0x19f32:1,0x19f33:1,0x19f34:1,0x19f35:1,0x19f36:1,0x19f37:1,0x19f38:1,0x19f39:1,0x19f3a:1,0x19f3c:1,0x19f3d:1,0x19f3e:1,0x19f3f:1,0x19f40:1,0x19f42:1,0x19f45:1,0x19f46:1,0x19f47:1,0x19f48:1,0x19f49:1,0x19f4a:1,0x19f4b:1,0x19f4c:1,0x19f4d:1,0x19f4e:1,0x19f4f:1,0x19f80:1,0x19f81:1,0x19f83:1,0x19f85:1,0x19f88:1,0x19f89:1,0x19f8a:1,0x19f8b:1,0x19f8c:1,0x19f8d:1,0x19f8e:1,0x19f8f:1,0x19f90:1,0x19f91:1,0x19f92:1,0x19f93:1,0x19f94:1,0x19f96:1,0x19f97:1,0x19f98:1,0x19f99:1,0x19f9b:1,0x19f9c:1,0x19f9d:1,0x19f9e:1,0x19f9f:1,0x19fa0:1,0x19fa2:1,0x19fa3:1,0x19fa4:1,0x19fa5:1,0x19fa6:1,0x19fa7:1,0x19fa8:1,0x19fa9:1,0x19faa:1,0x19fab:1,0x19fac:1,0x19fad:1,0x19fae:1,0x19faf:1,0x19fb0:1,0x19fb1:1,0x19fb2:1,0x19fb3:1,0x19fb4:1,0x19fb5:1,0x19fb6:1,0x19fb7:1,0x19fb8:1,0x19fb9:1,0x19fba:1,0x19fbb:1,0x19fbc:1,0x19fbd:1,0x19fbe:1,0x19fbf:1,0x19fc0:1,0x19fc1:1,0x19fc2:1,0x19fc3:1,0x19fc4:1,0x19fc5:1,0x19fc6:1,0x19fc7:1,0x19fc8:1,0x19fc9:1,0x19fca:1,0x19fcb:1,0x19fce:1,0x19fcf:1,0x19fd0:1,0x19fd1:1,0x19fd2:1,0x19fd3:1,0x19fd4:1,0x19fd5:1,0x19fd6:1,0x19fd8:1,0x19fd9:1,0x19fda:1,0x19fde:1,0x19fdf:1,0x19fe0:1,0x19fe1:1,0x19fe2:1,0x19fe3:1,0x19fe4:1,0x19fe5:1,0x19fe6:1,0x19fe7:1,0x19fe8:1,0x19fe9:1,0x19fea:1,0x19feb:1,0x19fec:1,0x19fed:1,0x19fee:1,0x19fef:1,0x19ff0:1,0x19ff1:1,0x19ff2:1,0x19ff3:1,0x19ff4:1,0x19ff7:1,0x19ff8:1,0x19ff9:1,0x19ffa:1,0x19ffb:1,0x19ffd:1,0x19ffe:1,0x19fff:1,0x1a800:1,0x1b888:1,0x1b8b0:1,0x1b8b2:1,0x1b92e:1,0x1b930:1,0x1b932:1,0x1b971:1,0x1b974:1,0x1ba94:1,0x1bab4:1,0x1bab5:1,0x1bab7:1,0x1bba0:1,0x1bd0f:1,0x1bd11:1,0x1bd6f:1,0x1bd73:1,0x1bddb:1,0x1bdde:1,0x1bf74:1,0x1bf7c:1,0x1bf7f:1,0x1bfad:1,0x1bfae:1,0x1c224:1,0x1c42f:1,0x1c457:1,0x1c4d2:1,0x1c4d3:1,0x1c4ef:1,0x1c609:1,0x1c715:1,0x1c741:1,0x1c751:1,0x1c752:1,0x1c87d:1,0x1c9b8:1,0x1c9bc:1,0x1c9be:1,0x1c9df:1,0x1ca35:1,0x1cb1b:1,0x1cc7d:1,0x1cca8:1,0x1cd15:1,0x1cd53:1,0x1cdf0:1,0x1ce99:1,0x1ceec:1,0x1d000:1,0x1d0c8:1,0x1d0ea:1,0x1d0eb:1,0x1d112:1,0x1d166:1,0x1d169:1,0x1d16a:1,0x1d304:1,0x1d306:1,0x1d316:1,0x1d31e:1,0x1d352:1,0x1d354:1,0x1d358:1,0x1d359:1,0x1d3f9:1,0x1d4d4:1,0x1d4d6:1,0x1d529:1,0x1d59c:1,0x1d59d:1,0x1d59e:1,0x1d5a2:1,0x1d5e7:1,0x1d90d:1,0x1d960:1,0x1d96f:1,0x1d998:1,0x1d99e:1,0x1d9fe:1,0x1db04:1,0x1db05:1,0x1dc08:1,0x1dc3e:1,0x1dc4b:1,0x1dc6e:1,0x1dc78:1,0x1dc7f:1,0x1dc9b:1,0x1dca1:1,0x1dca8:1,0x1dcaa:1,0x1dcab:1,0x1dcfc:1,0x1dd2f:1,0x1dd4b:1,0x1de52:1,0x1de83:1,0x1de85:1,0x1dff1:1,0x1dff3:1,0x1e120:1,0x1e160:1,0x1e220:1,0x1e23e:1,0x1e400:1,0x1e4b9:1,0x1e4c8:1,0x1e4d2:1,0x1e4d3:1,0x1e4da:1,0x1e4ea:1,0x1e597:1,0x1e723:1,0x1e82a:1,0x1e998:1,0x1e999:1,0x1ea01:1,0x1eb20:1,0x1eb24:1,0x1ebe0:1,0x1ebfd:1,0x1ecca:1,0x1eceb:1,0x1ed90:1,0x1edb2:1,0x1edb3:1,0x1ee21:1,0x1eec0:1,0x1eec1:1,0x1efcb:1,0x1eff9:1,0x1f054:1,0x1f0a1:1,0x1f0a3:1,0x1f1b0:1,0x1f1b5:1,0x1f759:1,0x224ed:1,0x22c14:1,0x22c15:1,0x24b10:1,0x24b11:1,0x25a06:1,0x25a07:1,0x25bc8:1,0x25bc9:1,0x25bca:1,0x25bcb:1,0x2804f:1,0x28050:1,0x28328:1,0x283b9:1,0x28cbc:1,0x28cd4:1,0x28cd5:1,0x28cd6:1,0x28cd7:1,0x29f73:1,0x2bdbd:1,0x2bdbe:1,0x2be79:1,0x2bec2:1,0x2d178:1,0x2d179:1,0x2d250:1,0x2d252:1,0x2d253:1,0x2d256:1,0x2d25a:1,0x2d2c9:1,0x2d2f6:1,0x2d323:1,0x2d34b:1,0x2d37b:1,0x2d3a5:1,0x2d3a6:1,0x2d3ad:1,0x2d3ae:1,0x2d842:1,0x2d843:1,0x2d85e:1,0x2d85f:1,0x2d8c1:1,0x2d8c8:1,0x2d8c9:1,0x2d8db:1,0x2da81:1,0x2dbb4:1,0x2dbbc:1,0x2dd3a:1,0x2dd46:1,0x2dd6e:1,0x2ded8:1,0x301f2:1,0x302f2:1,0x32801:1,0x32802:1,0x3280e:1,0x32812:1,0x32813:1,0x32817:1,0x32818:1,0x32819:1,0x3281a:1,0x32820:1,0x32821:1,0x32822:1,0x32823:1,0x32824:1,0x32829:1,0x32830:1,0x32831:1,0x32839:1,0x3283a:1,0x3283b:1,0x32851:1,0x32854:1,0x32856:1,0x3285b:1,0x3286e:1,0x32898:1,0x32899:1,0x3289a:1,0x328a0:1,0x328a2:1,0x328a4:1,0x328a6:1,0x328a7:1,0x328ad:1,0x328ae:1,0x328b0:1,0x328b1:1,0x328b2:1,0x328b4:1,0x328b8:1,0x328ba:1,0x328bb:1,0x328bd:1,0x328be:1,0x328e7:1,0x328e8:1,0x328e9:1,0x328ec:1,0x328ef:1,0x328f0:1,0x328f1:1,0x328f2:1,0x328f5:1,0x328f9:1,0x328fb:1,0x328fd:1,0x328fe:1,0x328ff:1,0x32904:1,0x32905:1,0x3290a:1,0x3290c:1,0x32914:1,0x32918:1,0x32919:1,0x3291b:1,0x3291c:1,0x32920:1,0x32921:1,0x32926:1,0x32927:1,0x32928:1,0x32929:1,0x3292b:1,0x3292f:1,0x32933:1,0x32935:1,0x32938:1,0x3293f:1,0x32943:1,0x32944:1,0x32946:1,0x3294f:1,0x32950:1,0x32957:1,0x3295b:1,0x3295d:1,0x32960:1,0x32964:1,0x32965:1,0x32967:1,0x32968:1,0x32969:1,0x3296b:1,0x3296c:1,0x3296d:1,0x3296e:1,0x3296f:1,0x32970:1,0x32973:1,0x32974:1,0x32977:1,0x32979:1,0x3297c:1,0x3297f:1,0x32981:1,0x32982:1,0x32983:1,0x32984:1,0x32985:1,0x32987:1,0x32988:1,0x32989:1,0x3298a:1,0x3298b:1,0x3298d:1,0x3298e:1,0x3298f:1,0x32990:1,0x32991:1,0x32992:1,0x32993:1,0x32995:1,0x32997:1,0x32998:1,0x3299a:1,0x3299b:1,0x3299c:1,0x3299d:1,0x3299f:1,0x329be:1,0x329c4:1,0x329c7:1,0x329c8:1,0x329cc:1,0x329d0:1,0x329d1:1,0x329d8:1,0x329dc:1,0x329dd:1,0x329e8:1,0x329e9:1,0x329ea:1,0x329ed:1,0x329f0:1,0x329f5:1,0x329f6:1,0x329fc:1,0x329fd:1,0x329fe:1,0x329ff:1,0x32a08:1,0x32a0b:1,0x32a0c:1,0x32a0f:1,0x32a14:1,0x32a18:1,0x32a1a:1,0x32a1b:1,0x32a20:1,0x32a23:1,0x32a27:1,0x32a32:1,0x32a36:1,0x32a38:1,0x32a3c:1,0x32a3d:1,0x32a4a:1,0x32a4b:1,0x32a4e:1,0x32a50:1,0x32a51:1,0x32a54:1,0x32a56:1,0x32a57:1,0x32a58:1,0x32a5b:1,0x32a5c:1,0x32a5e:1,0x32a62:1,0x32a64:1,0x32a77:1,0x32a7a:1,0x32a82:1,0x32a89:1,0x32a90:1,0x32a91:1,0x32a95:1,0x32a96:1,0x32a97:1,0x32a9b:1,0x32aa1:1,0x32aa2:1,0x32aaa:1,0x32aab:1,0x32aaf:1,0x32ab0:1,0x32ab4:1,0x32ab5:1,0x32ab7:1,0x32ab9:1,0x32ac3:1,0x32ace:1,0x32acf:1,0x32ad2:1,0x32ad3:1,0x32ad4:1,0x32ad5:1,0x32ad8:1,0x32adb:1,0x32af5:1,0x32af6:1,0x32afc:1,0x32afd:1,0x32c00:1,0x32c01:1,0x32c02:1,0x32c03:1,0x32c04:1,0x32c05:1,0x32c07:1,0x32c09:1,0x32c0a:1,0x32c0b:1,0x32c0d:1,0x32c0e:1,0x32c12:1,0x32c13:1,0x32c14:1,0x32c15:1,0x32c16:1,0x32c17:1,0x32c1a:1,0x32c1b:1,0x32c1e:1,0x32c1f:1,0x32c20:1,0x32c21:1,0x32c22:1,0x32c23:1,0x32c24:1,0x32c25:1,0x32c26:1,0x32c28:1,0x32c29:1,0x32c2d:1,0x32c2e:1,0x32c2f:1,0x32c30:1,0x32c31:1,0x32c32:1,0x32c33:1,0x32c34:1,0x32c35:1,0x32c36:1,0x32c37:1,0x32c38:1,0x32c39:1,0x32c3a:1,0x32c3b:1,0x32c3c:1,0x32c3d:1,0x32c3e:1,0x32c3f:1,0x32c40:1,0x32c42:1,0x32c43:1,0x32c44:1,0x32c45:1,0x32c46:1,0x32c47:1,0x32c48:1,0x32c49:1,0x32c4a:1,0x32c4b:1,0x32c4c:1,0x32c4d:1,0x32c4e:1,0x32c4f:1,0x32c50:1,0x32c51:1,0x32c52:1,0x32c53:1,0x32c54:1,0x32c55:1,0x32c56:1,0x32c57:1,0x32c58:1,0x32c59:1,0x32c5a:1,0x32c5b:1,0x32c5c:1,0x32c5d:1,0x32c5e:1,0x32c5f:1,0x32c60:1,0x32c61:1,0x32c62:1,0x32c63:1,0x32c64:1,0x32c65:1,0x32c66:1,0x32c67:1,0x32c68:1,0x32c69:1,0x32c6a:1,0x32c6b:1,0x32c6c:1,0x32c6d:1,0x32c6f:1,0x32c70:1,0x32c71:1,0x32c72:1,0x32c73:1,0x32c74:1,0x32c75:1,0x32c76:1,0x32c77:1,0x32c78:1,0x32c79:1,0x32c7a:1,0x32c7b:1,0x32c7c:1,0x32c7d:1,0x32c7e:1,0x32c7f:1,0x32c80:1,0x32c81:1,0x32c82:1,0x32c83:1,0x32c84:1,0x32c85:1,0x32c86:1,0x32c87:1,0x32c88:1,0x32c89:1,0x32c8a:1,0x32c8b:1,0x32cdc:1,0x32cdd:1,0x32cde:1,0x32cdf:1,0x32ce0:1,0x32ce1:1,0x32ce2:1,0x32ce3:1,0x32ce4:1,0x32ce5:1,0x32ce6:1,0x32ce7:1,0x32cf8:1,0x32cfa:1,0x32cfb:1,0x32d32:1,0x32d33:1,0x32d36:1,0x32d38:1,0x32d3a:1,0x32d3c:1,0x32d40:1,0x32d42:1,0x32d44:1,0x32d47:1,0x32d48:1,0x32d49:1,0x32d4b:1,0x32d4c:1,0x32d4f:1,0x32d58:1,0x32d59:1,0x32d5b:1,0x32d60:1,0x32d63:1,0x32d64:1,0x32d66:1,0x32d68:1,0x32d6a:1,0x32d6b:1,0x32d6c:1,0x32d6d:1,0x32d70:1,0x32d72:1,0x32d74:1,0x32d76:1,0x32d78:1,0x32d7c:1,0x32d7d:1,0x32d7f:1,0x32d8c:1,0x32d8d:1,0x32d90:1,0x32d91:1,0x32d93:1,0x32da0:1,0x32da5:1,0x32da6:1,0x32dba:1,0x32dbb:1,0x32dc9:1,0x32dcb:1,0x32ddb:1,0x32ddc:1,0x32ddd:1,0x32e00:1,0x32e01:1,0x32e03:1,0x32e04:1,0x32e08:1,0x32e10:1,0x32e1b:1,0x32e1d:1,0x32e1e:1,0x32e3b:1,0x32e41:1,0x32e44:1,0x32e51:1,0x32e55:1,0x32e61:1,0x32e62:1,0x32e64:1,0x32e78:1,0x32e81:1,0x32e82:1,0x32e83:1,0x32e84:1,0x32e86:1,0x32e87:1,0x32e9a:1,0x32e9c:1,0x32ea0:1,0x32ea8:1,0x32eac:1,0x32eaf:1,0x32eb8:1,0x32eb9:1,0x32ebe:1,0x32ec1:1,0x32ec2:1,0x32ee1:1,0x32eee:1,0x32ef4:1,0x32ef5:1,0x32ef7:1,0x32ef9:1,0x32efb:1,0x32efc:1,0x32efe:1,0x32f00:1,0x32f07:1,0x32f09:1,0x32f0d:1,0x32f10:1,0x32f2b:1,0x32f35:1,0x32f40:1,0x32f47:1,0x32f50:1,0x32f51:1,0x32f5f:1,0x32f66:1,0x32f7b:1,0x32f7c:1,0x34808:1,0x34814:1,0x34816:1,0x3481c:1,0x34839:1,0x3483a:1,0x3483b:1,0x3483c:1,0x3483d:1,0x34841:1,0x3485c:1,0x348e3:1,0x34922:1,0x34924:1,0x34928:1,0x34929:1,0x3492a:1,0x34930:1,0x34938:1,0x3493a:1,0x3493f:1,0x3495e:1,0x34b01:1,0x34d8d:1,0x34e6d:1,0x3698b:1,0x36991:1,0x369a2:1,0x369a3:1,0x36ae7:1,0x36d69:1,0x36e58:1,0x36e59:1,0x36e5c:1,0x3727b:1,0x373de:1,0x3741c:1,0x3741d:1,0x3742f:1,0x37435:1,0x37617:1,0x3771c:1,0x3771f:1,0x37a8c:1,0x37be6:1,0x37c6e:1,0x37c77:1,0x37e08:1,0x37f7e:1,0x37f7f:1,0x37fff:1
};

)wtf";
extern const std::string ss_cnip_whiteIpList = R"wtf(var whiteIpList = {
	"188.188.188.188":1,
	"100.100.100.100":1
};

)wtf";
extern const std::string ss_cnip_subnetIpRangeList = R"wtf(var subnetIpRangeList = [
0,1,
167772160,184549376,	//10.0.0.0/8
2886729728,2887778304,	//172.16.0.0/12
3232235520,3232301056,	//192.168.0.0/16
2130706432,2130706688	//127.0.0.0/24
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
	for ( var i = 0; i < 10; i += 2 ) {
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

	var strIp = dnsResolve(host);
	if ( !strIp ) {
		return wall_proxy();
	}
	
	return getProxyFromIP(strIp);
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
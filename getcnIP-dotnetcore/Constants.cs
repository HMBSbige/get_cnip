using System.Text;
using System.Text.Encodings.Web;
using System.Text.Json;

namespace getcnIP
{
	public static class Constants
	{
		public const string ApnicUrl = @"https://ftp.apnic.net/apnic/stats/apnic/delegated-apnic-latest";
		public const string IpipNetUrl = @"https://raw.githubusercontent.com/17mon/china_ip_list/master/china_ip_list.txt";
		public const string AcceleratedDomainsChinaUrl = @"https://raw.githubusercontent.com/felixonmars/dnsmasq-china-list/master/accelerated-domains.china.conf";
		public const string AppleChinaUrl = @"https://raw.githubusercontent.com/felixonmars/dnsmasq-china-list/master/apple.china.conf";
		public const string GoogleChinaUrl = @"https://raw.githubusercontent.com/felixonmars/dnsmasq-china-list/master/google.china.conf";
		public const string GfwlistUrl = @"https://raw.githubusercontent.com/gfwlist/gfwlist/master/gfwlist.txt";

		public static readonly UTF8Encoding UTF8withoutBOM = new UTF8Encoding(false);

		public const string Path = @"output";
		public const string Filename_chndomains = @"chndomains.txt";
		public const string Filename_cnip = @"chn_ip.txt";
		public const string Filename_addroute = @"add.txt";
		public const string Filename_delroute = @"del.txt";
		public const string Filename_ss_cnall = @"ss_cnall.pac";
		public const string Filename_ss_cnip = @"ss_cnip.pac";
		public const string Filename_ss_white = @"ss_white.pac";
		public const string Filename_ss_white_r = @"ss_white_r.pac";
		public const string Filename_whitelist_acl = @"whitelist.acl";

		public const string ApnicPath = @"delegated-apnic-latest";
		public const string IpipNetPath = @"china_ip_list.txt";
		public const string AcceleratedDomainsChinaPath = @"accelerated-domains.china.conf";
		public const string AppleChinaPath = @"apple.china.conf";
		public const string GoogleChinaPath = @"google.china.conf";
		public const string WhiteListPath = @"whitelist.txt";

		public static readonly JsonSerializerOptions Options = new JsonSerializerOptions
		{
			WriteIndented = false,
			Encoder = JavaScriptEncoder.Default
		};
	}
}

using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Text;
using System.Text.RegularExpressions;

namespace getcnIP_dotnetcore
{
	internal static class GetCNIP
	{
		public const string ApnicPath = @"delegated-apnic-latest";
		public const string IpipNetPath = @"china_ip_list.txt";

		private static KeyValuePair<IPAddress, int>? GetCNIPv4InfoFromApnicLine(string str)
		{
			if (string.IsNullOrWhiteSpace(str))
			{
				return null;
			}

			var strA = str.Split('|');
			//apnic|CN|ipv4|
			if (strA.Length > 4 && strA[0] == @"apnic" && strA[1] == @"CN" && strA[2] == @"ipv4")
			{
				return new KeyValuePair<IPAddress, int>(IPAddress.Parse(strA[3]), Convert.ToInt32(strA[4]));
			}

			return null;
		}

		private static KeyValuePair<IPAddress, int>? GetCNIPv4InfoFromIpipNetLine(string str)
		{
			if (string.IsNullOrWhiteSpace(str))
			{
				return null;
			}

			var reg = new Regex("^(.+)/(.+)$");
			var match = reg.Match(str);
			if (match.Groups.Count == 3)
			{
				var ipv4 = IPAddress.Parse(match.Groups[1].Value);
				var hosts = IPv4Subnet.CIDR2Hosts(Convert.ToInt32(match.Groups[2].Value));
				return new KeyValuePair<IPAddress, int>(ipv4, hosts);
			}

			return null;
		}

		public static Dictionary<IPAddress, int> ReadFromIpipNet()
		{
			if (!File.Exists(IpipNetPath))
			{
				return ReadFromApnic();
			}
			var ipv4Subnet = new Dictionary<IPAddress, int>();
			using (var sr = new StreamReader(IpipNetPath, Encoding.UTF8))
			{
				string line;
				while ((line = sr.ReadLine()) != null)
				{
					var p = GetCNIPv4InfoFromIpipNetLine(line);
					if (p != null)
					{
						ipv4Subnet.Add(p.Value.Key, p.Value.Value);
					}
				}
			}

			return ipv4Subnet.Count == 0 ? null : ipv4Subnet;
		}

		public static Dictionary<IPAddress, int> ReadFromApnic()
		{
			if (!File.Exists(ApnicPath))
			{
				return null;
			}

			var ipv4Subnet = new Dictionary<IPAddress, int>();
			using (var sr = new StreamReader(ApnicPath, Encoding.UTF8))
			{
				string line;
				while ((line = sr.ReadLine()) != null)
				{
					var p = GetCNIPv4InfoFromApnicLine(line);
					if (p != null)
					{
						ipv4Subnet.Add(p.Value.Key, p.Value.Value);
					}
				}
			}

			return ipv4Subnet.Count == 0 ? null : ipv4Subnet;
		}
	}
}

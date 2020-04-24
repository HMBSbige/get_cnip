using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Text.RegularExpressions;

namespace getcnIP
{
	internal static class GetCNIP
	{
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

		public static Dictionary<IPAddress, int> ReadFromIpipNet(string str)
		{
			return str.GetLines().Select(GetCNIPv4InfoFromIpipNetLine).Where(p => p != null).ToDictionary(p => p.Value.Key, p => p.Value.Value);
		}

		public static Dictionary<IPAddress, int> ReadFromApnic(string str)
		{
			return str.GetLines().Select(GetCNIPv4InfoFromApnicLine).Where(p => p != null).ToDictionary(p => p.Value.Key, p => p.Value.Value);
		}
	}
}

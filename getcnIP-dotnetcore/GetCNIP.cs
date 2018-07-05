using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Text;
using System.Text.RegularExpressions;

namespace getcnIP_dotnetcore
{
	internal class IPv4Subnet
	{
		#region data
		public readonly IPAddress Netmask;
		public readonly IPAddress Wildcard;
		public readonly int CIDR;
		public readonly int Hosts;
		public readonly IPAddress FirstIP;
		public readonly IPAddress LastIP;
		#endregion

		private static uint IPv42UintLE(IPAddress ipv4)
		{
			var buf = ipv4.GetAddressBytes();
			return BitConverter.ToUInt32(buf, 0);
		}

		public static uint IPv42UintBE(IPAddress ipv4)
		{
			var buf = ipv4.GetAddressBytes();
			Array.Reverse(buf);
			return BitConverter.ToUInt32(buf, 0);
		}

		private static IPAddress IPv4BinStrToIPv4(string str)
		{
			var bytesAddress = new[]
			{
					Convert.ToByte(str.Substring(0, 8),2),
					Convert.ToByte(str.Substring(8, 8),2),
					Convert.ToByte(str.Substring(16, 8),2),
					Convert.ToByte(str.Substring(24, 8),2)
			};
			return new IPAddress(bytesAddress);
		}

		private static string IPv4ToIPv4BinStr(IPAddress ipv4)
		{
			var bytesAddress = ipv4.GetAddressBytes();

			return $@"{Convert.ToString(bytesAddress[0], 2).PadLeft(8, '0')}{
					   Convert.ToString(bytesAddress[1], 2).PadLeft(8, '0')}{
					   Convert.ToString(bytesAddress[2], 2).PadLeft(8, '0')}{
					   Convert.ToString(bytesAddress[3], 2).PadLeft(8, '0')}";
		}

		public static int Hosts2CIDR(int hosts)
		{
			return 32 - Convert.ToInt32(Math.Log(hosts, 2));
		}

		public static int CIDR2Hosts(int CIDR)
		{
			return Convert.ToInt32(Math.Pow(2, 32 - Convert.ToInt32(CIDR)));
		}

		public IPv4Subnet(IPAddress ipv4, int hosts)
		{
			Hosts = hosts;
			CIDR = Hosts2CIDR(hosts);

			var netmaskStr = new string('1', CIDR) + new string('0', 32 - CIDR);
			Netmask = IPv4BinStrToIPv4(netmaskStr);
			var netmaskUint = IPv42UintLE(Netmask);

			var wildcardStr = new string('0', CIDR) + new string('1', 32 - CIDR);
			Wildcard = IPv4BinStrToIPv4(wildcardStr);
			var wildcardUint = IPv42UintLE(Wildcard);

			var ipv4Uint = IPv42UintLE(ipv4);

			FirstIP = new IPAddress(ipv4Uint & netmaskUint);
			LastIP = new IPAddress(ipv4Uint | wildcardUint);
		}
	}

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

		private static Dictionary<IPAddress, int> ReadFromApnic()
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

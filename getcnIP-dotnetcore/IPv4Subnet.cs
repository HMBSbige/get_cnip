using System;
using System.Net;
using System.Text.RegularExpressions;

namespace getcnIP
{
	internal class IPv4Subnet
	{
		#region data
		public readonly IPAddress Netmask;//子网掩码
		public readonly IPAddress Wildcard;//~子网掩码
		public readonly int CIDR;//无类别域间路由
		public readonly int Hosts;//主机数
		public readonly IPAddress FirstIP;//IP段的第一个IP地址
		public readonly IPAddress LastIP; //IP段的最后一个IP地址
		#endregion

		private static readonly Regex Ipv4Pattern = new Regex("^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){1}(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\\.){2}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$");

		public static bool IsIPv4Address(string input)
		{
			return Ipv4Pattern.IsMatch(input);
		}

		/// <summary>
		/// IPv4地址转整数(小端)
		/// </summary>
		/// <param name="ipv4">IPv4地址</param>
		/// <returns>IPv4地址所代表的小端整数</returns>
		public static uint IPv42UintLE(IPAddress ipv4)
		{
			var buf = ipv4.GetAddressBytes();
			return BitConverter.ToUInt32(buf, 0);
		}

		/// <summary>
		/// IPv4地址转整数(大端)
		/// </summary>
		/// <param name="ipv4">IPv4地址</param>
		/// <returns>IPv4地址所代表的大端整数</returns>
		public static uint IPv42UintBE(IPAddress ipv4)
		{
			var buf = ipv4.GetAddressBytes();
			Array.Reverse(buf);
			return BitConverter.ToUInt32(buf, 0);
		}

		/// <summary>
		/// 将表示二进制的字符串转为IPv4
		/// </summary>
		/// <param name="str">二进制字符串</param>
		/// <returns>IPv4地址</returns>
		public static IPAddress IPv4BinStrToIPv4(string str)
		{
			var bytesAddress = new[]
			{
					Convert.ToByte(str.Substring(0, 8), 2),
					Convert.ToByte(str.Substring(8, 8), 2),
					Convert.ToByte(str.Substring(16, 8), 2),
					Convert.ToByte(str.Substring(24, 8), 2)
			};
			return new IPAddress(bytesAddress);
		}

		/// <summary>
		/// 将IPv4转为二进制字符串
		/// </summary>
		/// <param name="ipv4">IPv4地址</param>
		/// <returns>二进制字符串</returns>
		public static string IPv4ToIPv4BinStr(IPAddress ipv4)
		{
			var bytesAddress = ipv4.GetAddressBytes();

			return
					$@"{Convert.ToString(bytesAddress[0], 2).PadLeft(8, '0')}{
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

		/// <summary>
		/// 构造函数
		/// </summary>
		/// <param name="ipv4">IPv4地址</param>
		/// <param name="hosts">该IP段主机数</param>
		public IPv4Subnet(IPAddress ipv4, int hosts)
		{
			Hosts = hosts;
			CIDR = 32 - Convert.ToInt32(Math.Log(hosts, 2));

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
}

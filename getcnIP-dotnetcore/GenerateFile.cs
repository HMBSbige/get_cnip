using System;
using System.Collections.Generic;
using System.IO;
using System.Net;
using System.Text;

namespace getcnIP_dotnetcore
{
	static class GenerateFile
	{
		private static readonly UTF8Encoding UTF8withoutBOM = new UTF8Encoding(false);
		public const string Path = @".\output\";
		public const string Filename_chndomains = @"chndomains.txt";
		public const string Filename_cnip = @"chn_ip.txt";
		public const string Filename_addroute = @"add.txt";
		public const string Filename_delroute = @"del.txt";
		public const string Filename_ss_cnip = @"ss_cnip.pac";
		public const string Filename_ss_white = @"ss_white.pac";
		public const string Filename_ss_white_r = @"ss_white_r.pac";
		public const string Filename_whitelist_acl = @"whitelist.acl";

		#region private
		
		private static string GetcnIpRange(Dictionary<IPAddress, int> ipv4Subnets)
		{
			var sb = new StringBuilder("[\n{");

			uint startNum = 0;
			var comma = @"";
			foreach (var ipv4Subnet in ipv4Subnets)
			{
				var p = new IPv4Subnet(ipv4Subnet.Key, ipv4Subnet.Value);

				while (IPv4Subnet.IPv42UintBE(p.FirstIP) >> 24 > startNum)
				{
					++startNum;
					sb.Append(@"},{");
					comma = @"";
				}

				sb.Append(comma);
				sb.Append(@"0x");
				sb.Append(Convert.ToString(IPv4Subnet.IPv42UintBE(p.FirstIP) / 256, 16));
				sb.Append(@":");
				sb.Append(ipv4Subnet.Value / 256);
				comma = @",";
			}
			sb.Append("}\n];");
			return sb.ToString();
		}

		private static string GetcnIp16Range(Dictionary<IPAddress, int> ipv4Subnets)
		{
			var sb = new StringBuilder("{\n");

			var masterNetSet = new SortedSet<uint>();
			foreach (var ipv4Subnet in ipv4Subnets)
			{
				var p = new IPv4Subnet(ipv4Subnet.Key, ipv4Subnet.Value);
				if (ipv4Subnet.Value < 1 << 14)
				{
					masterNetSet.Add(IPv4Subnet.IPv42UintBE(p.FirstIP) >> 14);
				}
			}
			var masterNet = new List<uint>(masterNetSet.Count);
			foreach (var x in masterNetSet)
			{
				masterNet.Add(x);
			}
			//masterNet.Sort();
			foreach (var x in masterNet)
			{
				sb.Append(@"0x");
				sb.Append(Convert.ToString(x, 16));
				sb.Append(@":1,");
			}
			sb.Remove(sb.Length - 1, 1);
			sb.Append("\n};");
			return sb.ToString();
		}

		private static string GetACLCNIP(Dictionary<IPAddress, int> ipv4Subnets)
		{
			var sb = new StringBuilder();
			foreach (var ipv4Subnet in ipv4Subnets)
			{
				var p = new IPv4Subnet(ipv4Subnet.Key, ipv4Subnet.Value);
				sb.AppendLine($@"{p.FirstIP}/{p.CIDR}");
			}
			return sb.ToString();
		}

		private static string GetPACwhitedomains(IEnumerable<string> domains)
		{
			var m = new SortedDictionary<string, HashSet<string>>();
			foreach (var domain in domains)
			{
				var lastIndexOfdot = domain.LastIndexOf('.', domain.Length - 1);
				if (lastIndexOfdot == -1)
				{
					continue;
				}
				var secondlevel = domain.Remove(lastIndexOfdot);
				var toplevel = domain.Substring(lastIndexOfdot + 1);
				if (!string.IsNullOrWhiteSpace(secondlevel) && !string.IsNullOrWhiteSpace(toplevel))
				{
					if (m.ContainsKey(toplevel))
					{
						m[toplevel].Add(secondlevel);
					}
					else
					{
						m[toplevel] = new HashSet<string> { secondlevel };
					}
				}
			}

			OutputLog(m);

			var sb = new StringBuilder();
			foreach (var domain in m)
			{
				var ssb = new StringBuilder();
				foreach (var seconddomain in domain.Value)
				{
					ssb.AppendFormat("\"{0}\":1,\n", seconddomain);
				}
				ssb.Remove(ssb.Length - 2, 2);
				ssb.Append("\n");
				sb.AppendFormat("\"{0}\":{{\n{1}}},", domain.Key, ssb);
			}
			sb.Remove(sb.Length - 1, 1);
			return sb.ToString();
		}

		private static string GetACLwhitedomains(IEnumerable<string> domains)
		{
			var sb=new StringBuilder();
			foreach (var domain in domains)
			{
				sb.AppendLine($@"^(.*\.)?{domain.Replace(@".", @"\.")}$");
			}
			return sb.ToString();
		}

		private static void OutputLog(SortedDictionary<string, HashSet<string>> list)
		{
			Console.WriteLine();
			foreach (var domains in list)
			{
				Console.WriteLine($@"{domains.Key}:{domains.Value.Count}");
			}
			Console.WriteLine();
		}

		#endregion

		#region public
		
		public static void Writechndomains(IEnumerable<string> domains)
		{
			using (var fileS = new FileStream(Path + Filename_chndomains, FileMode.Create,FileAccess.Write))
			{
				using (var sw = new StreamWriter(fileS, UTF8withoutBOM))
				{
					foreach (var domain in domains)
					{
						sw.WriteLine(domain);
					}
				}
			}
		}

		public static void Writecnip(Dictionary<IPAddress, int> ipv4Subnets)
		{
			using (var fileS = new FileStream(Path + Filename_cnip, FileMode.Create, FileAccess.Write))
			{
				using (var sw = new StreamWriter(fileS, UTF8withoutBOM))
				{
					foreach (var ipv4Subnet in ipv4Subnets)
					{
						var p=new IPv4Subnet(ipv4Subnet.Key, ipv4Subnet.Value);

						sw.WriteLine($@"{p.FirstIP} {p.LastIP}");
					}
				}
			}
		}

		public static void Writeaddroute(Dictionary<IPAddress, int> ipv4Subnets)
		{
			using (var fileS = new FileStream(Path + Filename_addroute, FileMode.Create, FileAccess.Write))
			{
				using (var sw = new StreamWriter(fileS, UTF8withoutBOM))
				{
					foreach (var ipv4Subnet in ipv4Subnets)
					{
						var p = new IPv4Subnet(ipv4Subnet.Key, ipv4Subnet.Value);

						sw.WriteLine($@"add {p.FirstIP} mask {p.Netmask} default METRIC default IF default");
					}
				}
			}
		}

		public static void Writedelroute(Dictionary<IPAddress, int> ipv4Subnets)
		{
			using (var fileS = new FileStream(Path + Filename_delroute, FileMode.Create, FileAccess.Write))
			{
				using (var sw = new StreamWriter(fileS, UTF8withoutBOM))
				{
					foreach (var ipv4Subnet in ipv4Subnets)
					{
						var p = new IPv4Subnet(ipv4Subnet.Key, ipv4Subnet.Value);

						sw.WriteLine($@"delete {p.FirstIP} mask {p.Netmask} default METRIC default IF default");
					}
				}
			}
		}
		
		public static void Writesscnip(Dictionary<IPAddress, int> ipv4Subnets, IEnumerable<string> domains)
		{
			var sb=new StringBuilder(StringResource.ss_cnip_template);
			sb.Replace(@"__cnIpRange__", GetcnIpRange(ipv4Subnets));
			sb.Replace(@"__cnIp16Range__", GetcnIp16Range(ipv4Subnets));
			sb.Replace(@"__white_domains__", GetPACwhitedomains(domains));
			using (var fileS = new FileStream(Path + Filename_ss_cnip, FileMode.Create, FileAccess.Write))
			{
				using (var sw = new StreamWriter(fileS, UTF8withoutBOM))
				{
					sw.Write(sb);
				}
			}
		}

		public static void Writesswhite(Dictionary<IPAddress, int> ipv4Subnets, IEnumerable<string> domains)
		{
			var sb = new StringBuilder(StringResource.ss_white_template);
			sb.Replace(@"__cnIpRange__", GetcnIpRange(ipv4Subnets));
			sb.Replace(@"__cnIp16Range__", GetcnIp16Range(ipv4Subnets));
			sb.Replace(@"__white_domains__", GetPACwhitedomains(domains));
			using (var fileS = new FileStream(Path + Filename_ss_white, FileMode.Create, FileAccess.Write))
			{
				using (var sw = new StreamWriter(fileS, UTF8withoutBOM))
				{
					sw.Write(sb);
				}
			}
		}

		public static void Writesswhiter(Dictionary<IPAddress, int> ipv4Subnets, IEnumerable<string> domains)
		{
			var sb = new StringBuilder(StringResource.ss_white_r_template);
			sb.Replace(@"__cnIpRange__", GetcnIpRange(ipv4Subnets));
			sb.Replace(@"__cnIp16Range__", GetcnIp16Range(ipv4Subnets));
			sb.Replace(@"__white_domains__", GetPACwhitedomains(domains));
			using (var fileS = new FileStream(Path + Filename_ss_white_r, FileMode.Create, FileAccess.Write))
			{
				using (var sw = new StreamWriter(fileS, UTF8withoutBOM))
				{
					sw.Write(sb);
				}
			}
		}

		public static void Write_whitelist_acl(Dictionary<IPAddress, int> ipv4Subnets, IEnumerable<string> domains)
		{
			var sb = new StringBuilder(StringResource.acl_whitelist_template);
			sb.Replace(@"__white_domains__", GetACLwhitedomains(domains));
			sb.Replace(@"__CNIP__", GetACLCNIP(ipv4Subnets));
			using (var fileS = new FileStream(Path + Filename_whitelist_acl, FileMode.Create, FileAccess.Write))
			{
				using (var sw = new StreamWriter(fileS, UTF8withoutBOM))
				{
					sw.Write(sb);
				}
			}
		}
		
		#endregion
	}
}

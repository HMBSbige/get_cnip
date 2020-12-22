using NUglify;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Text.Json;
using System.Threading.Tasks;

namespace getcnIP
{
	static class GenerateFile
	{
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
			masterNet.AddRange(masterNetSet);
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
			var m = new SortedDictionary<string, SortedDictionary<string, int>>();
			foreach (var domain in domains)
			{
				var lastIndexOfdot = domain.LastIndexOf('.', domain.Length - 1);
				if (lastIndexOfdot == -1)
				{
					continue;
				}
				var secondLevel = domain.Remove(lastIndexOfdot);
				var topLevel = domain.Substring(lastIndexOfdot + 1);
				if (!string.IsNullOrWhiteSpace(secondLevel) && !string.IsNullOrWhiteSpace(topLevel))
				{
					if (m.TryGetValue(topLevel, out var v))
					{
						v[secondLevel] = 1;
					}
					else
					{
						m[topLevel] = new SortedDictionary<string, int> { [secondLevel] = 1 };
					}
				}
			}

			OutputLog(m);
			return JsonSerializer.Serialize(m, Constants.Options);
		}

		private static string GetACLwhitedomains(IEnumerable<string> domains)
		{
			var sb = new StringBuilder();
			foreach (var domain in domains)
			{
				sb.AppendLine($@"^(.*\.)?{domain.Replace(@".", @"\.")}$");
			}
			return sb.ToString();
		}

		private static void OutputLog(SortedDictionary<string, SortedDictionary<string, int>> list)
		{
			foreach (var domains in list)
			{
				Debug.WriteLine($@"{domains.Key}:{domains.Value.Count}");
			}
		}

		#endregion

		#region public

		public static async Task Writecnip(Dictionary<IPAddress, int> ipv4Subnets)
		{
			Console.WriteLine($@"正在生成 {Constants.Filename_cnip}...");
			var path = Path.Combine(Constants.Path, Constants.Filename_cnip);

			var sb = new StringBuilder();
			foreach (var p in ipv4Subnets.Select(ipv4Subnet => new IPv4Subnet(ipv4Subnet.Key, ipv4Subnet.Value)))
			{
				sb.Append($"{p.FirstIP} {p.LastIP}\n");
			}

			await File.WriteAllTextAsync(path, sb.ToString(), Constants.UTF8withoutBOM);
		}

		public static async Task Writeaddroute(Dictionary<IPAddress, int> ipv4Subnets)
		{
			Console.WriteLine($@"正在生成 {Constants.Filename_addroute}...");
			var path = Path.Combine(Constants.Path, Constants.Filename_addroute);

			var sb = new StringBuilder();
			foreach (var p in ipv4Subnets.Select(ipv4Subnet => new IPv4Subnet(ipv4Subnet.Key, ipv4Subnet.Value)))
			{
				sb.Append($"add {p.FirstIP} mask {p.Netmask} default METRIC default IF default\n");
			}

			await File.WriteAllTextAsync(path, sb.ToString(), Constants.UTF8withoutBOM);
		}

		public static async Task Writedelroute(Dictionary<IPAddress, int> ipv4Subnets)
		{
			Console.WriteLine($@"正在生成 {Constants.Filename_delroute}...");
			var path = Path.Combine(Constants.Path, Constants.Filename_delroute);

			var sb = new StringBuilder();
			foreach (var p in ipv4Subnets.Select(ipv4Subnet => new IPv4Subnet(ipv4Subnet.Key, ipv4Subnet.Value)))
			{
				sb.Append($"delete {p.FirstIP} mask {p.Netmask} default METRIC default IF default\n");
			}

			await File.WriteAllTextAsync(path, sb.ToString(), Constants.UTF8withoutBOM);
		}

		public static async Task Writechndomains(IEnumerable<string> domains)
		{
			Console.WriteLine($@"正在生成 {Constants.Filename_chndomains}...");
			var path = Path.Combine(Constants.Path, Constants.Filename_chndomains);

			var sb = new StringBuilder();
			foreach (var domain in domains)
			{
				sb.Append($"{domain}\n");
			}

			await File.WriteAllTextAsync(path, sb.ToString(), Constants.UTF8withoutBOM);
		}

		public static async Task Writesscnall(Dictionary<IPAddress, int> ipv4Subnets, IEnumerable<string> domains)
		{
			Console.WriteLine($@"正在生成 {Constants.Filename_ss_cnall}...");
			var path = Path.Combine(Constants.Path, Constants.Filename_ss_cnall);

			var sb = new StringBuilder(Resource.ss_cnip_temp);
			sb.Replace(@"__cnIpRange__", GetcnIpRange(ipv4Subnets));
			sb.Replace(@"__cnIp16Range__", GetcnIp16Range(ipv4Subnets));
			sb.Replace(@"__white_domains__", GetPACwhitedomains(domains));

			await File.WriteAllTextAsync(path, sb.ToString(), Constants.UTF8withoutBOM);

			await MinifiedJs(path);
		}

		public static async Task Writesscnip(Dictionary<IPAddress, int> ipv4Subnets, IEnumerable<string> domains)
		{
			Console.WriteLine($@"正在生成 {Constants.Filename_ss_cnip}...");
			var path = Path.Combine(Constants.Path, Constants.Filename_ss_cnip);

			var sb = new StringBuilder(Resource.ss_cnip_temp);
			sb.Replace(@"__cnIpRange__", GetcnIpRange(ipv4Subnets));
			sb.Replace(@"__cnIp16Range__", GetcnIp16Range(ipv4Subnets));
			sb.Replace(@"__white_domains__", GetPACwhitedomains(domains));

			await File.WriteAllTextAsync(path, sb.ToString(), Constants.UTF8withoutBOM);

			await MinifiedJs(path);
		}

		public static async Task Writesswhite(Dictionary<IPAddress, int> ipv4Subnets, IEnumerable<string> domains)
		{
			Console.WriteLine($@"正在生成 {Constants.Filename_ss_white}...");
			var path = Path.Combine(Constants.Path, Constants.Filename_ss_white);

			var sb = new StringBuilder(Resource.ss_white_temp);
			sb.Replace(@"__cnIpRange__", GetcnIpRange(ipv4Subnets));
			sb.Replace(@"__cnIp16Range__", GetcnIp16Range(ipv4Subnets));
			sb.Replace(@"__white_domains__", GetPACwhitedomains(domains));

			await File.WriteAllTextAsync(path, sb.ToString(), Constants.UTF8withoutBOM);

			await MinifiedJs(path);
		}

		public static async Task Writesswhiter(Dictionary<IPAddress, int> ipv4Subnets, IEnumerable<string> domains)
		{
			Console.WriteLine($@"正在生成 {Constants.Filename_ss_white_r}...");
			var path = Path.Combine(Constants.Path, Constants.Filename_ss_white_r);

			var sb = new StringBuilder(Resource.ss_white_r_temp);
			sb.Replace(@"__cnIpRange__", GetcnIpRange(ipv4Subnets));
			sb.Replace(@"__cnIp16Range__", GetcnIp16Range(ipv4Subnets));
			sb.Replace(@"__white_domains__", GetPACwhitedomains(domains));

			await File.WriteAllTextAsync(path, sb.ToString(), Constants.UTF8withoutBOM);

			await MinifiedJs(path);
		}

		public static async Task Write_whitelist_acl(Dictionary<IPAddress, int> ipv4Subnets, IEnumerable<string> domains)
		{
			Console.WriteLine($@"正在生成 {Constants.Filename_whitelist_acl}...");
			var path = Path.Combine(Constants.Path, Constants.Filename_whitelist_acl);

			var sb = new StringBuilder(Resource.whitelist_temp);
			sb.Replace(@"__white_domains__", GetACLwhitedomains(domains));
			sb.Replace(@"__CNIP__", GetACLCNIP(ipv4Subnets));

			await File.WriteAllTextAsync(path, sb.ToString(), Constants.UTF8withoutBOM);
		}

		private static async Task MinifiedJs(string path)
		{
			if (!File.Exists(path))
			{
				return;
			}

			var oldSize = Utils.GetFileSize(path);

			var js = await File.ReadAllTextAsync(path, Constants.UTF8withoutBOM);
			var result = Uglify.Js(js);
			var minifiedString = result.Code;
			await File.WriteAllTextAsync(path, minifiedString, Constants.UTF8withoutBOM);

			var newSize = Utils.GetFileSize(path);

			Console.WriteLine($@"{Path.GetFileName(path)} ({Utils.BytesToString(oldSize)} => {Utils.BytesToString(newSize)})");
		}

		#endregion
	}
}

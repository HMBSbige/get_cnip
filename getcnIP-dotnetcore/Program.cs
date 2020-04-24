using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;

namespace getcnIP
{
	internal static class Program
	{
		private static string apnicStr = string.Empty;
		private static string ipipNetStr = string.Empty;
		private static string AcceleratedDomainsChinaStr = string.Empty;
		private static string AppleChinaStr = string.Empty;
		private static string GoogleChinaStr = string.Empty;

		private static async Task Main(string[] args)
		{
			if (args.Contains(@"--local"))
			{
				await LoadFiles();
			}
			else
			{
				await LoadFilesFromInternetAsync();
			}

			await Do();
		}

		private static async Task Do()
		{
			var chnIpv4subnets_IpipNet = GetCNIP.ReadFromIpipNet(ipipNetStr);
			var chnIpv4subnets_Apnic = GetCNIP.ReadFromApnic(apnicStr);
			Console.WriteLine($@"Apnic:共有{chnIpv4subnets_Apnic.Count}条。");
			Console.WriteLine($@"IpipNet:共有{chnIpv4subnets_IpipNet.Count}条。");
			if (chnIpv4subnets_IpipNet.Count == 0)
			{
				chnIpv4subnets_IpipNet = chnIpv4subnets_Apnic;
			}

			Directory.CreateDirectory(Constants.Path);

			await GenerateFile.Writecnip(chnIpv4subnets_IpipNet);
			await GenerateFile.Writeaddroute(chnIpv4subnets_IpipNet);
			await GenerateFile.Writedelroute(chnIpv4subnets_IpipNet);

			var d1 = GetCNDomains.Read(AcceleratedDomainsChinaStr);
			var d2 = GetCNDomains.Read(AppleChinaStr);
			var d3 = GetCNDomains.Read(GoogleChinaStr);
			var userWhiteList = (await GetCNDomains.ReadFromFile(Constants.WhiteListPath)).ToArray();

			var domains = new HashSet<string>();
			domains.UnionWith(d1);
			domains.UnionWith(d2);
			domains.UnionWith(d3);
			domains.UnionWith(userWhiteList);
			Console.WriteLine($@"共有域名 {domains.Count} 条，其中用户自定义 {userWhiteList.Length} 条");

			await GenerateFile.Writechndomains(domains);
			await GenerateFile.Writesscnall(chnIpv4subnets_Apnic, domains);
			await GenerateFile.Writesscnip(chnIpv4subnets_Apnic, userWhiteList);
			await GenerateFile.Writesswhite(chnIpv4subnets_Apnic, domains);
			await GenerateFile.Writesswhiter(chnIpv4subnets_Apnic, domains);
			await GenerateFile.Write_whitelist_acl(chnIpv4subnets_IpipNet, domains);

			Console.WriteLine(@"done");
		}

		private static async Task LoadFiles()
		{
			apnicStr = await Utils.ReadTextAsync(Constants.ApnicPath);
			ipipNetStr = await Utils.ReadTextAsync(Constants.IpipNetPath);
			AcceleratedDomainsChinaStr = await Utils.ReadTextAsync(Constants.AcceleratedDomainsChinaPath);
			AppleChinaStr = await Utils.ReadTextAsync(Constants.AppleChinaPath);
			GoogleChinaStr = await Utils.ReadTextAsync(Constants.GoogleChinaPath);
		}

		private static async Task LoadFilesFromInternetAsync()
		{
			apnicStr = await HttpRequest.GetAsync(Constants.ApnicUrl);
			ipipNetStr = await HttpRequest.GetAsync(Constants.IpipNetUrl);
			AcceleratedDomainsChinaStr = await HttpRequest.GetAsync(Constants.AcceleratedDomainsChinaUrl);
			AppleChinaStr = await HttpRequest.GetAsync(Constants.AppleChinaUrl);
			GoogleChinaStr = await HttpRequest.GetAsync(Constants.GoogleChinaUrl);
		}
	}
}

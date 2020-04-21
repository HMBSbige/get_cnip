using System;
using System.IO;
using System.Linq;

namespace getcnIP_dotnetcore
{
	internal static class Program
	{
		private static void CheckAndCreateDirectory(string path)
		{
			Directory.CreateDirectory(path);
		}
		private static void Main()
		{
			Console.WriteLine(@"正在分析 CNIPv4...");
			var chnipv4subnets_IpipNet = GetCNIP.ReadFromIpipNet();
			var chnipv4subnets_Apnic = GetCNIP.ReadFromApnic();
			if (chnipv4subnets_Apnic != null)
			{
				CheckAndCreateDirectory(GenerateFile.Path);
				Console.WriteLine($@"Apnic:共有{chnipv4subnets_Apnic.Count}条。");
				Console.WriteLine($@"IpipNet:共有{chnipv4subnets_IpipNet.Count}条。");
				Console.WriteLine($@"正在生成 {GenerateFile.Filename_cnip},{GenerateFile.Filename_addroute},{GenerateFile.Filename_delroute}...");
				try
				{
					GenerateFile.Writecnip(chnipv4subnets_IpipNet);
					GenerateFile.Writeaddroute(chnipv4subnets_IpipNet);
					GenerateFile.Writedelroute(chnipv4subnets_IpipNet);
				}
				catch (Exception ex)
				{
					Console.WriteLine(ex.Message);
					return;
				}
				Console.WriteLine(@"生成成功！" + Environment.NewLine);

				Console.WriteLine(@"正在分析 CNDomains...");
				var idomains = GetCNDomains.ReadFromFile(false);
				var idomains2 = GetCNDomains.ReadFromFile(true);
				if (idomains != null)
				{
					var domains = idomains.ToList();
					Console.WriteLine($@"共有{domains.Count}条。");
					Console.WriteLine($@"正在生成 {GenerateFile.Filename_chndomains},{GenerateFile.Filename_ss_cnall},{GenerateFile.Filename_ss_cnip},{GenerateFile.Filename_ss_white},{GenerateFile.Filename_ss_white_r},{GenerateFile.Filename_whitelist_acl}...");
					try
					{
						GenerateFile.Writechndomains(domains);
						GenerateFile.Writesscnall(chnipv4subnets_Apnic, domains);
						GenerateFile.Writesscnip(chnipv4subnets_Apnic, idomains2);
						GenerateFile.Writesswhite(chnipv4subnets_Apnic, domains);
						GenerateFile.Writesswhiter(chnipv4subnets_Apnic, domains);
						GenerateFile.Write_whitelist_acl(chnipv4subnets_IpipNet, domains);
					}
					catch (Exception ex)
					{
						Console.WriteLine(ex.Message);
						return;
					}
					Console.WriteLine(@"生成成功！");
				}
				else
				{
					Console.WriteLine($@"未找到 {GetCNDomains.Path1} 或 {GetCNDomains.Path2},程序退出");
				}
			}
			else
			{
				Console.WriteLine($@"未找到 {GetCNIP.IpipNetPath} 或 {GetCNIP.ApnicPath},程序退出");
			}
		}
	}
}

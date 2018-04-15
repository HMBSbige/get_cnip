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
			var chnipv4subnets = GetCNIP.ReadFromFile();
	        if (chnipv4subnets != null)
	        {
		        CheckAndCreateDirectory(GenerateFile.Path);
				Console.WriteLine($@"共有{chnipv4subnets.Count}条。");
				Console.WriteLine($@"正在生成 {GenerateFile.Filename_cnip},{GenerateFile.Filename_addroute},{GenerateFile.Filename_delroute}...");
		        try
		        {
			        GenerateFile.Writecnip(chnipv4subnets);
			        GenerateFile.Writeaddroute(chnipv4subnets);
			        GenerateFile.Writedelroute(chnipv4subnets);
		        }
		        catch (Exception ex)
		        {
					Console.WriteLine(ex.Message);
			        return;
		        }
				Console.WriteLine(@"生成成功！"+Environment.NewLine);

		        Console.WriteLine(@"正在分析 CNDomains...");
				var idomains = GetCNDomains.ReadFromFile();
				if (idomains != null)
		        {
					var domains = idomains.ToList();
			        Console.WriteLine($@"共有{domains.Count}条。");
			        Console.WriteLine($@"正在生成 {GenerateFile.Filename_chndomains},{GenerateFile.Filename_ss_cnip},{GenerateFile.Filename_ss_white},{GenerateFile.Filename_ss_white_r},{GenerateFile.Filename_whitelist_acl}...");
			        try
			        {
				        GenerateFile.Writechndomains(domains);
				        GenerateFile.Writesscnip(chnipv4subnets, domains);
				        GenerateFile.Writesswhite(chnipv4subnets, domains);
				        GenerateFile.Writesswhiter(chnipv4subnets, domains);
				        GenerateFile.Write_whitelist_acl(chnipv4subnets, domains);
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
		        Console.WriteLine($@"未找到 {GetCNIP.Path},程序退出");
	        }
		}
    }
}

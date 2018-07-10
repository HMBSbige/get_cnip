using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Text.RegularExpressions;

namespace getcnIP_dotnetcore
{
	internal static class GetCNDomains
	{
		public const string Path1 = @"accelerated-domains.china.conf";
		public const string Path2 = @"whitelist.txt";

		private static string GetDomainFromLine(string str)
		{
			var reg = new Regex("^server=/(.+)/(.+)$");
			var match = reg.Match(str);
			return match.Groups[1].Value.ToLower();
		}

		public static IEnumerable<string> ReadFromFile()
		{
			if (!File.Exists(Path1) && !File.Exists(Path2))
			{
				return null;
			}

			var domains = new HashSet<string>();

			if (File.Exists(Path2))
			{
				using (var sr = new StreamReader(Path2, Encoding.UTF8))
				{
					string line;
					while ((line = sr.ReadLine()) != null)
					{
						var domain = line;
						if (!string.IsNullOrWhiteSpace(domain))
						{
							domains.Add(domain);
						}
					}
				}
			}

			if (File.Exists(Path1))
			{
				using (var sr = new StreamReader(Path1, Encoding.UTF8))
				{
					string line;
					while ((line = sr.ReadLine()) != null)
					{
						var domain = GetDomainFromLine(line);
						if (!string.IsNullOrWhiteSpace(domain))
						{
							domains.Add(domain);
						}
					}
				}
			}

			return domains.Count == 0 ? null : domains;
		}
	}
}

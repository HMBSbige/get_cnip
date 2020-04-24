using System.Collections.Generic;
using System.IO;
using System.Text.RegularExpressions;
using System.Threading.Tasks;

namespace getcnIP
{
	internal static class GetCNDomains
	{
		private static string GetDomainFromLine(string str)
		{
			str = str.Trim();
			if (str.StartsWith('#'))
			{
				return string.Empty;
			}
			var reg = new Regex("^server=/(.+)/(.+)$");
			var match = reg.Match(str);
			var res = match.Groups[1].Value.ToLower();
			return string.IsNullOrWhiteSpace(res) ? str : res;
		}

		public static async Task<IEnumerable<string>> ReadFromFile(string path)
		{
			if (File.Exists(path))
			{
				var str = await File.ReadAllTextAsync(path, Constants.UTF8withoutBOM);
				return Read(str);
			}
			return new List<string>();
		}

		public static IEnumerable<string> Read(string str)
		{
			var domains = new HashSet<string>();

			foreach (var line in str.GetLines())
			{
				var domain = GetDomainFromLine(line);
				if (!string.IsNullOrWhiteSpace(domain))
				{
					domains.Add(domain);
				}
			}

			return domains;
		}
	}
}

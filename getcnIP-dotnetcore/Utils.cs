using System.Collections.Generic;
using System.IO;
using System.Threading.Tasks;

namespace getcnIP
{
	public static class Utils
	{
		public static IEnumerable<string> GetLines(this string str, bool removeEmptyLines = true)
		{
			using var sr = new StringReader(str);
			string line;
			while ((line = sr.ReadLine()) != null)
			{
				if (removeEmptyLines && string.IsNullOrWhiteSpace(line))
				{
					continue;
				}
				yield return line;
			}
		}

		public static async Task<string> ReadTextAsync(string path)
		{
			if (File.Exists(path))
			{
				return await File.ReadAllTextAsync(path, Constants.UTF8withoutBOM);
			}
			return string.Empty;
		}
	}
}

using System;
using System.Collections.Generic;
using System.Globalization;
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

		public static long GetFileSize(string path)
		{
			return new FileInfo(path).Length;
		}

		public static string BytesToString(long byteCount)
		{
			string[] suf = {"B", "KB", "MB", "GB", "TB", "PB", "EB"}; //Longs run out around EB
			if (byteCount == 0)
				return $@"0 {suf[0]}";
			var bytes = Math.Abs(byteCount);
			var place = Convert.ToInt32(Math.Floor(Math.Log(bytes, 1024)));
			var num = Math.Round(bytes / Math.Pow(1024, place), 1);
			return $@"{(Math.Sign(byteCount) * num).ToString(CultureInfo.InvariantCulture)} {suf[place]}";
		}
	}
}

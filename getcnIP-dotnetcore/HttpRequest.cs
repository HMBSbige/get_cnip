using System;
using System.Net.Http;
using System.Threading.Tasks;

namespace getcnIP
{
	public class HttpRequest
	{
		protected const string DefaultUserAgent = @"Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/81.0.4044.113 Safari/537.36";
		protected const int DefaultGetTimeout = 30000;

		public static async Task<string> GetAsync(string url, double timeout = DefaultGetTimeout)
		{
			Console.WriteLine($@"正在加载 {url}...");
			var httpClientHandler = new HttpClientHandler();
			var httpClient = new HttpClient(httpClientHandler)
			{
				Timeout = TimeSpan.FromMilliseconds(timeout),
				DefaultRequestVersion = new Version(2, 0)
			};
			var request = new HttpRequestMessage(HttpMethod.Get, url);
			request.Headers.Add(@"User-Agent", DefaultUserAgent);

			var response = await httpClient.SendAsync(request);

			response.EnsureSuccessStatusCode();
			var resultStr = await response.Content.ReadAsStringAsync();
			return resultStr;
		}
	}
}

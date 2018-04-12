using System.Collections.Generic;
using System.IO;
using System.Text;

namespace getcnIP_dotnetcore
{
	internal static class GetCNDomains
    {
	    private const string Path1 = @"accelerated-domains.china.conf";
	    private const string Path2 = @"whitelist.txt";

	    private static string GetDomainFromLine(string str)
	    {
		    var sArray = str.Split('/');
		    return sArray[1];
	    }

		public static IEnumerable<string> ReadFromFile()
	    {
		    if (!File.Exists(Path1) && !File.Exists(Path2))
		    {
			    return null;
		    }

			var domains=new List<string>();
			
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

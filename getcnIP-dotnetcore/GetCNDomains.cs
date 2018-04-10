using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace getcnIP_dotnetcore
{
	internal class GetCNDomains
    {
	    private const string Path1 = @"accelerated-domains.china.conf";
	    private const string Path2 = @"whitelist.txt";

	    private string GetDomainFromLine(string str)
	    {
		    var sArray = str.Split('/');
		    return sArray[1];
	    }

		public List<string> Read()
	    {
		    if (!File.Exists(Path1) && !File.Exists(Path2))
		    {
			    return null;
		    }

			var domains=new List<string>();

		    if (File.Exists(Path1))
		    {
			    var sr = new StreamReader(Path1, Encoding.UTF8);
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
			
		    if (File.Exists(Path2))
		    {
			    var sr = new StreamReader(Path2, Encoding.UTF8);
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

		    return domains.Count == 0 ? null : domains;
	    }
    }
}

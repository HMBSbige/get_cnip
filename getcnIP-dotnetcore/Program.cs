using System;
using System.IO;

namespace getcnIP_dotnetcore
{
    class Program
    {
        static void Main(string[] args)
        {
	        GetCNDomains g=new GetCNDomains();
	        var listofdomains = g.Read();

			if (listofdomains == null)
	        {
				Console.WriteLine(Directory.GetCurrentDirectory());
	        }
	        else
	        {
				Console.WriteLine(listofdomains[0]);
		        Console.WriteLine(listofdomains.Count);
			}

        }
    }
}

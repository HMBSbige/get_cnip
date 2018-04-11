namespace getcnIP_dotnetcore
{
	internal static class Program
    {
        static void Main(string[] args)
        {
			GenerateFile.Writesscnip(GetCNIP.Read(),GetCNDomains.Read());
        }
    }
}

namespace getcnIP_dotnetcore
{
	internal static class Program
    {
        static void Main(string[] args)
        {
			GenerateFile.Write_whitelist_acl(GetCNIP.ReadFromFile(),GetCNDomains.ReadFromFile());
        }
    }
}

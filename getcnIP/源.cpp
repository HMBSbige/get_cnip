#include <iostream>
#include <string>
#include <io.h>
#include <direct.h>
#include <locale.h>
#include "generatefile.h"
using namespace std;

/////////////////////////////////////////////////////////////////////////////

int main() {
	std::ios::sync_with_stdio(false);
	_wsetlocale(LC_ALL, L"");

	if (_access(string(R"(.\out)").c_str(), 0) == -1)//判断文件夹是否存在
	{
		_mkdir(string(R"(.\out)").c_str());//system(("md " + PATH + "\\out").c_str());
	}

	getcnip();

	gfwlist2pac();

	get_cn_domains();

	generate_user_dot_rule();

	generate_ss_cnip();

	generate_ss_white();

	generate_ss_white_r();

	generate_local_cnip();

	cout << endl;
	//system("pause");
	return 0;
}

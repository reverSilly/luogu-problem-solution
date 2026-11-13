#include<bits/stdc++.h>
using namespace std;
int main()
{
	ifstream fin("in.txt");
	string a,b,c;
	fin>>a>>b>>c;
	printf("[%s][%s][%s]\n",a.c_str(),b.c_str(),c.c_str());
	ifstream fo("o1.txt");
	string ans;
	fo>>ans;
	printf("ans=[%s]\n",ans.c_str());
	return 0;
}

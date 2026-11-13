#include<bits/stdc++.h>
using namespace std;
int main()
{
	static mt19937 rng(9);
	for(int t=0;t<3;++t)
	{
		string s;
		for(int i=0;i<100;++i)s+=(char)('a'+rng()%2);
		printf("%s\n",s.c_str());
	}
	return 0;
}

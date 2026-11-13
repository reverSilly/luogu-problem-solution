#include<bits/stdc++.h>
using namespace std;
int main()
{
	static mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
	int la=1+rng()%10,lb=1+rng()%8,lc=1+rng()%8;
	int al=2+rng()%2; // 字母表大小 2 或 3
	string s[3];
	int len[3]={la,lb,lc};
	for(int t=0;t<3;++t)
		for(int i=0;i<len[t];++i)s[t]+=(char)('a'+rng()%al);
	printf("%s\n%s\n%s\n",s[0].c_str(),s[1].c_str(),s[2].c_str());
	return 0;
}

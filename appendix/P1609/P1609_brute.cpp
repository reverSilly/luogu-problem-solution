//
// P1609 暴力（对拍用）：从 N+1 开始逐个枚举, 找到第一个回文数
#include<bits/stdc++.h>
using namespace std;
using u64=unsigned long long;

bool pal(u64 x)
{
	string s=to_string(x);
	string t=s;
	reverse(t.begin(),t.end());
	return s==t;
}

int main()
{
	string s;
	cin>>s;
	u64 n=0;
	for(char ch:s)
		n=n*10+(ch-'0');
	for(u64 x=n+1;;++x)
		if(pal(x))
		{
			cout<<x<<'\n';
			return 0;
		}
}

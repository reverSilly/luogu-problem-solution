//
// 题目：P1643 完美数
// 暴力基准：从 X+1 开始逐个递增整数检验是否为十进制回文数，直到找齐第 N 个
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

bool is_pal(i64 x)
{
	string s=to_string(x);
	int l=0,r=(int)s.size()-1;
	while(l<r)
		if(s[l++]!=s[r--])
			return false;
	return true;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int T;
	cin>>T;
	while(T--)
	{
		i64 x,n;
		cin>>x>>n;
		while(n>0)
		{
			++x;
			if(is_pal(x))
				--n;
		}
		cout<<x<<'\n';
	}
	return 0;
}

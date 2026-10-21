//
// 题目：P1630 求和 （暴力对拍用）
// 直接逐项求幂累加（用 __int128 保证不溢出），最后取模 1e4
#include<bits/stdc++.h>
using namespace std;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int t;
	cin>>t;
	while(t--)
	{
		long long a,b;
		cin>>a>>b;
		__int128 s=0;
		for(long long i=1;i<=a;++i)
		{
			__int128 p=1;
			for(long long j=0;j<b;++j)
				p*=i;
			s+=p;
		}
		cout<<(long long)(s%10000)<<'\n';
	}
	return 0;
}

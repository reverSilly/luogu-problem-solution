//
// P1590 朴素解：直接枚举 1..n, 逐位判断是否含 7
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int t;
	cin>>t;
	while(t--)
	{
		i64 n;
		cin>>n;
		i64 cnt=0;
		for(i64 x=1;x<=n;++x)
		{
			i64 y=x;
			bool ok=true;
			while(y)
			{
				if(y%10==7)
				{
					ok=false;
					break;
				}
				y/=10;
			}
			if(ok)
				++cnt;
		}
		cout<<cnt<<'\n';
	}
	return 0;
}

//
// P1592 朴素解：从 1 开始逐个判断 gcd, 数到第 k 个
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	i64 n,k;
	cin>>n>>k;
	i64 cnt=0;
	for(i64 x=1;;++x)
		if(gcd(x,n)==1)
		{
			++cnt;
			if(cnt==k)
			{
				cout<<x<<'\n';
				break;
			}
		}
	return 0;
}

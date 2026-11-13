// 题目：P2214 [USACO14MAR] Mooo Moo S
// own[i] = h[i] - max(0, h[i-1]-1) 唯一确定；再对每个 own 做完全背包求最少头数
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105},mxv{100005},inf{1145141919};

int n,b,v[mxn],h[mxn],dp[mxv];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>b;
	for(int i=0;i<b;++i)
		cin>>v[i];
	for(int i=0;i<n;++i)
		cin>>h[i];
	// 全局数组默认 0，必须显式填无穷
	fill_n(dp,mxv,inf);
	dp[0]=0;
	for(int x=1;x<mxv;++x)
		for(int i=0;i<b;++i)
			if(v[i]<=x&&dp[x-v[i]]+1<dp[x])
				dp[x]=dp[x-v[i]]+1;
	int ans=0;
	for(int i=0;i<n;++i)
	{
		int own=h[i]-(i?max(0,h[i-1]-1):0);
		if(own<0||own>=mxv||dp[own]>=inf)
		{
			cout<<-1<<'\n';
			return 0;
		}
		ans+=dp[own];
	}
	cout<<ans<<'\n';
	return 0;
}

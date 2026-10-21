//
// 题目：P1642 规划
// 暴力基准：枚举大小恰为 n-m 的点子集，BFS 检验连通性并取最大比值
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{20};

int n,m,k,w[mxn],c[mxn];
vector<int>graph[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	k=n-m;
	for(int i=0;i<n;++i)
		cin>>w[i];
	for(int i=0;i<n;++i)
		cin>>c[i];
	for(int i=1,u,v;i<n;++i)
	{
		cin>>u>>v;--u;--v;
		graph[u].push_back(v);
		graph[v].push_back(u);
	}

	double ans=0;
	for(int mask=1;mask<(1<<n);++mask)
	{
		if(__builtin_popcount(mask)!=k)
			continue;
		int s=__builtin_ctz(mask);
		int vis=1<<s,q[mxn], l=0, r=0;
		q[r++]=s;
		while(l<r)
		{
			int u=q[l++];
			for(int v:graph[u])
				if((mask>>v&1)&&!(vis>>v&1))
				{
					vis|=1<<v;
					q[r++]=v;
				}
		}
		if(vis!=mask)
			continue;
		double sw=0,sc=0;
		for(int i=0;i<n;++i)
			if(mask>>i&1)
				sw+=w[i],sc+=c[i];
		ans=max(ans,sw/sc);
	}
	cout<<fixed<<setprecision(1)<<ans<<'\n';
	return 0;
}

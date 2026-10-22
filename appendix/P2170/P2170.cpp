// 题目：P2170 选学霸
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{20005};

int fa[mxn],sz[mxn],cnt[mxn];
bitset<mxn>dp;

int getrt(int x)
{
	if(fa[x]==x)
		return x;
	return fa[x]=getrt(fa[x]);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n,m,k;
	cin>>n>>m>>k;
	for(int i=1;i<=n;++i)
		fa[i]=i,sz[i]=1;
	for(int i=1,u,v;i<=k;++i)
	{
		cin>>u>>v;
		int x=getrt(u),y=getrt(v);
		if(x!=y)
			fa[x]=y,sz[y]+=sz[x];
	}
	// 实力相同的人被并查集合并成一组，整组只能全选或全不选
	for(int i=1;i<=n;++i)
		if(fa[i]==i)
			++cnt[sz[i]];
	dp[0]=1;
	for(int s=1;s<=n;++s)
		for(int t=0;t<cnt[s];++t)
			dp|=dp<<s;
	// 在所有可达人数里取与 m 最接近者，差相同取较小者
	int ans=0,best=1145141919;
	for(int t=0;t<=n;++t)
		if(dp[t]&&abs(t-m)<best)
			best=abs(t-m),ans=t;
	cout<<ans<<'\n';
	return 0;
}

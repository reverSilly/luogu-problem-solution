// 题目：P2847 [USACO16DEC] Moocast G
// 答案 = 平方距离完全图的 MST 最大边（MST 即最小瓶颈生成树）
// 稠密 Prim O(N^2)，不存边；哨兵必须 > 2*25000^2=1.25e9，1145141919 不够大！
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1005};
constexpr i64 inf{1145141919810};

int n,x[mxn],y[mxn];
i64 dis[mxn];
bool vis[mxn];

i64 d2(int u,int v)
{
	i64 dx=x[u]-x[v],dy=y[u]-y[v];
	return dx*dx+dy*dy;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>x[i]>>y[i];
	fill_n(dis,n,inf);	// 全局默认 0 是合法边权，必须显式填 INF
	dis[0]=0;
	i64 ans=0;
	for(int t=0;t<n;++t)
	{
		int u=-1;
		for(int i=0;i<n;++i)
			if(!vis[i]&&(u==-1||dis[i]<dis[u]))
				u=i;
		vis[u]=1;
		ans=max(ans,dis[u]);	// 瓶颈 = MST 最大边
		for(int v=0;v<n;++v)
			if(!vis[v])
				dis[v]=min(dis[v],d2(u,v));
	}
	cout<<ans<<'\n';
	return 0;
}

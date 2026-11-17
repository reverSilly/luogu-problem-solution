// 题目：P2912 [USACO08OCT] Pasture Walking G
// 树上两点路径必过 LCA：dist(u,v)=dis[u]+dis[v]-2*dis[lca]。
// dis = 到根的带权距离；倍增 gf[u][k] = u 的 2^k 级祖先（越过根停在根）。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005},mxk{11};	// 2^10=1024>1000

int n,q,dep[mxn],dis[mxn],gf[mxn][mxk];
vector<pair<int,int>>graph[mxn];

void dfs(int u,int fa)
{
	gf[u][0]=fa;
	for(int k=1;k<mxk;++k)
		gf[u][k]=gf[gf[u][k-1]][k-1];
	for(auto[v,w]:graph[u])
		if(v!=fa)
		{
			dep[v]=dep[u]+1;
			dis[v]=dis[u]+w;
			dfs(v,u);
		}
}

int lca(int u,int v)
{
	if(dep[u]<dep[v])
		swap(u,v);
	for(int k=mxk-1;k>=0;--k)	// 先把深的抬到同深度
		if(dep[gf[u][k]]>=dep[v])
			u=gf[u][k];
	if(u==v)
		return u;
	for(int k=mxk-1;k>=0;--k)	// 一起上跳，停在 LCA 的正下方
		if(gf[u][k]!=gf[v][k])
			u=gf[u][k],v=gf[v][k];
	return gf[u][0];
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>q;
	for(int i=1,u,v,w;i<n;++i)
	{
		cin>>u>>v>>w;--u;--v;
		graph[u].push_back({v,w});
		graph[v].push_back({u,w});
	}
	dfs(0,0);	// 根的父亲设为自己：跳越根时钳在根，无需 INF 哨兵
	for(int i=0,u,v;i<q;++i)
	{
		cin>>u>>v;--u;--v;
		cout<<dis[u]+dis[v]-2*dis[lca(u,v)]<<'\n';
	}
	return 0;
}

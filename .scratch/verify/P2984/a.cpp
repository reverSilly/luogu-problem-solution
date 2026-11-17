// 题目：P2984 [USACO10FEB] Chocolate Giving S
// 必经点 1 的最短路 = d(1,P)+d(1,Q)，一次 Dijkstra 预处理，O(1) 回答每个询问
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{50005},inf{1145141919};

int n,m,b,dis[mxn];
bool vis[mxn];
vector<pair<int,int>>graph[mxn];

void dij()
{
	fill_n(dis,n,inf);	// 全局数组默认 0，必须显式填 INF
	priority_queue<pair<int,int>,vector<pair<int,int>>,greater<>>Q;
	dis[0]=0;Q.emplace(0,0);
	while(!Q.empty())
	{
		auto[d,u]=Q.top();Q.pop();
		if(vis[u])continue;	// 惰性删除：过期堆元素直接丢
		vis[u]=true;
		for(auto[v,w]:graph[u])
			if(dis[u]+w<dis[v])
			{
				dis[v]=dis[u]+w;
				Q.emplace(dis[v],v);
			}
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>b;
	for(int i=0,u,v,w;i<m;++i)
	{
		cin>>u>>v>>w;--u;--v;
		graph[u].emplace_back(v,w);
		graph[v].emplace_back(u,w);
	}
	dij();
	for(int i=0,p,q;i<b;++i)
	{
		cin>>p>>q;--p;--q;
		cout<<dis[p]+dis[q]<<'\n';	// 最大 2*(N-1)*2000 < 2^31，int 安全
	}
	return 0;
}
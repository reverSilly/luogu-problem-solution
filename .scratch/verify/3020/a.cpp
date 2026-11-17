// 题目：P3020 [USACO11MAR] Package Delivery S
// 非负边权无向图 1->N 最短路 = 最少点心；堆优化 Dijkstra，惰性删除
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{50005},inf{1145141919};

int n,m,dis[mxn];
bool vis[mxn];
vector<pair<int,int>>graph[mxn];	// {v,w}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0,u,v,w;i<m;++i)
	{
		cin>>u>>v>>w;--u;--v;
		graph[u].push_back({v,w});
		graph[v].push_back({u,w});
	}
	fill_n(dis,n,inf);		// 全局默认 0 不是 INF！必须显式填
	dis[0]=0;
	priority_queue<pair<int,int>,vector<pair<int,int>>,greater<>>Q;
	Q.push({0,0});
	while(!Q.empty())
	{
		auto[d,u]=Q.top();Q.pop();
		if(vis[u])
			continue;
		vis[u]=true;		// 出堆即定型（边权非负）
		for(auto[v,w]:graph[u])
			if(dis[v]>d+w)
			{
				dis[v]=d+w;
				Q.push({dis[v],v});
			}
	}
	cout<<dis[n-1]<<'\n';
	return 0;
}

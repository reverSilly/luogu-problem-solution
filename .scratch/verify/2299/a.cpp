// 题目：P2299 Mzc和体委的争夺战
// 思路：双向正权图，求 1 到 n 的最短路 => Dijkstra + 小根堆（惰性删除）
//       最短路最多 n-1 条边、每条 1e6，长度会超 int，距离必须 64 位
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{2505};
constexpr i64 inf{1145141919810};

struct edge{int v,w;};

int n,m;
vector<edge>graph[mxn];
i64 dis[mxn];
bool vis[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0,a,b,c;i<m;++i)
	{
		cin>>a>>b>>c;
		--a;--b;
		graph[a].push_back({b,c});
		graph[b].push_back({a,c});
	}

	fill_n(dis,mxn,inf);
	priority_queue<pair<i64,int>,vector<pair<i64,int>>,greater<>>Q;
	dis[0]=0;
	Q.emplace(0,0);
	while(!Q.empty())
	{
		auto[d,u]=Q.top();
		Q.pop();
		if(vis[u])
			continue;      // 惰性删除：过期的堆记录直接丢
		vis[u]=true;
		for(auto[v,w]:graph[u])
			if(dis[u]+w<dis[v])
			{
				dis[v]=dis[u]+w;
				Q.emplace(dis[v],v);
			}
	}
	cout<<dis[n-1]<<'\n';
	return 0;
}

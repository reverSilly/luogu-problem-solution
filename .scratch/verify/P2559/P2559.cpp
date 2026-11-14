// 题目：P2559 [AHOI2002] 哈利·波特与魔法石
// 无向图，边权 = h[t]（该地形有魔法石则减半），Dijkstra 求起点到终点最短路
// 城市编号 ≤ 100 但未必连续，直接用编号作数组下标
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105},inf{1145141919};

int s[8],dis[mxn];
bool vis[mxn];
vector<pair<int,int>>graph[mxn];	// {邻点, 边权}
int h[8]={0,2,6,4,8,6,10,14};		// h[1..7]，h[0] 占位不用

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	for(int i=1;i<=7;++i)
		cin>>s[i];
	int st,ed,c;
	cin>>st>>ed>>c;
	for(int i=0,u,v,t;i<c;++i)
	{
		cin>>u>>v>>t;
		int w=s[t]?h[t]/2:h[t];		// 有魔法石减半，h 全为偶数故整除
		graph[u].push_back({v,w});
		graph[v].push_back({u,w});
	}
	fill_n(dis,mxn,inf);
	dis[st]=0;
	priority_queue<pair<int,int>,vector<pair<int,int>>,greater<>>Q;
	Q.push({0,st});
	while(!Q.empty())
	{
		auto[d,u]=Q.top();Q.pop();
		if(vis[u])
			continue;
		vis[u]=true;
		for(auto[v,w]:graph[u])
			if(dis[u]+w<dis[v])
			{
				dis[v]=dis[u]+w;
				Q.push({dis[v],v});
			}
	}
	cout<<dis[ed]<<'\n';
	return 0;
}
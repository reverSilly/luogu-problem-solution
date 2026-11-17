// 题目：P2951 [USACO09OPEN] Hide and Seek S
// 无权无向图 BFS：dis=距 1 号谷仓的最短边数；再扫一遍求最大距离、最小编号、个数
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{50005},inf{1145141919};

int n,m,dis[mxn],que[mxn];
vector<int>graph[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0,u,v;i<m;++i)
	{
		cin>>u>>v;--u;--v;
		graph[u].push_back(v);
		graph[v].push_back(u);
	}
	fill_n(dis,n,inf);	// 全局默认 0 是合法距离，必须显式置 INF
	dis[0]=0;
	int hd=0,tl=0;
	que[tl++]=0;
	while(hd<tl)
	{
		int u=que[hd++];
		for(int v:graph[u])
			if(dis[v]==inf)
			{
				dis[v]=dis[u]+1;
				que[tl++]=v;
			}
	}
	int mx=-1,id=0,cnt=0;
	for(int i=0;i<n;++i)
	{
		if(dis[i]==inf)	// 不可达不参与统计（题面保证连通，此分支只是保险）
			continue;
		if(dis[i]>mx)
		{
			mx=dis[i];id=i;cnt=1;
		}
		else if(dis[i]==mx)
			++cnt;
	}
	cout<<id+1<<' '<<mx<<' '<<cnt<<'\n';
	return 0;
}
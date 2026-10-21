//
// 题目：P1640 [SCOI2010] 连续攻击游戏
// 暴力基准：二分/逐个增广匈牙利算法求从 1 开始的最大连续匹配
#include<bits/stdc++.h>
using namespace std;

constexpr int mxv{10005};

int n,match_r[mxv],vis[mxv],stamp=0;
vector<int>graph[mxv];

bool dfs(int u)
{
	for(int v:graph[u])
	{
		if(vis[v]==stamp)
			continue;
		vis[v]=stamp;
		if(match_r[v]==-1||dfs(match_r[v]))
		{
			match_r[v]=u;
			return true;
		}
	}
	return false;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	fill_n(match_r,n,-1);
	for(int i=0,u,v;i<n;++i)
	{
		cin>>u>>v;
		graph[u].push_back(i);
		if(v!=u)
			graph[v].push_back(i);
	}

	int ans=0;
	for(int i=1;i<=10000;++i)
	{
		++stamp;
		if(dfs(i))
			ans=i;
		else
			break;
	}
	cout<<ans<<'\n';
	return 0;
}

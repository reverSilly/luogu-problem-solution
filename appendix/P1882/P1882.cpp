//
// 题目：P1882 [USACO10OPEN] 接力赛跑 B
// https://www.luogu.com.cn/problem/P1882
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};
constexpr int inf{1145141919};

vector<int> g[mxn];
int len[mxn],stt[mxn];
bool vis[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	int n;
	cin>>n;
	for(int i=0;i<n;++i)
	{
		int m;
		cin>>len[i]>>m;
		g[i].clear();
		while(m--)
		{
			int v;
			cin>>v;
			--v;
			g[i].push_back(v);
		}
		stt[i]=inf;
	}
	stt[0]=0;
	priority_queue<pair<int,int>,vector<pair<int,int>>,greater<pair<int,int>>> pq;
	pq.push({0,0});
	int ans=0;
	while(!pq.empty())
	{
		auto [t,u]=pq.top();
		pq.pop();
		if(vis[u]||t!=stt[u])
			continue;
		vis[u]=true;
		int ft=t+len[u];
		ans=max(ans,ft);
		for(int v:g[u])
		{
			if(vis[v])
				continue;
			if(ft<stt[v])
			{
				stt[v]=ft;
				pq.push({ft,v});
			}
		}
	}
	cout<<ans<<'\n';
	return 0;
}

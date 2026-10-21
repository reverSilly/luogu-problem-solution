//
// 题目：P1629 邮递员送信
// https://www.luogu.com.cn/problem/P1629
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1005};
constexpr i64 inf{1145141919810LL};

int n,m;
vector<pair<int,i64>>g[mxn],rg[mxn];
i64 dis[mxn],rdis[mxn];
bool vis[mxn];

void dij(int s,vector<pair<int,i64>>*gr,i64*d)
{
	fill_n(d,mxn,inf);
	fill_n(vis,mxn,false);
	priority_queue<pair<i64,int>,vector<pair<i64,int>>,greater<>>Q;
	d[s]=0;
	Q.emplace(0,s);
	while(!Q.empty())
	{
		auto[dd,u]=Q.top();
		Q.pop();
		if(vis[u])
			continue;
		vis[u]=true;
		for(auto[v,w]:gr[u])
			if(d[v]>dd+w)
			{
				d[v]=dd+w;
				Q.emplace(d[v],v);
			}
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<m;++i)
	{
		int u,v;
		i64 w;
		cin>>u>>v>>w;--u;--v;
		g[u].push_back({v,w});
		rg[v].push_back({u,w});          // 反向图：求所有点到 1 的最短路
	}
	dij(0,g,dis);
	dij(0,rg,rdis);
	i64 ans=0;
	for(int i=1;i<n;++i)
		ans+=dis[i]+rdis[i];
	cout<<ans<<'\n';
	return 0;
}

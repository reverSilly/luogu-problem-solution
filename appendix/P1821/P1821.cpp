//
// 题目：P1821 [USACO07FEB] Cow Party S
// https://www.luogu.com.cn/problem/P1821
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1005};
constexpr i64 inf{1145141919810LL};

int n,m,x;
vector<pair<int,int>>g[mxn],rg[mxn];
i64 dis[mxn],rdis[mxn];
bool vis[mxn];

void dij(int s,vector<pair<int,int>>*gr,i64*d)
{
	fill_n(d,n,inf);
	fill_n(vis,n,false);
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
	cin>>n>>m>>x;
	--x;
	for(int i=0,u,v,w;i<m;++i)
	{
		cin>>u>>v>>w;
		--u;--v;
		g[u].push_back({v,w});
		rg[v].push_back({u,w});
	}
	dij(x,g,dis);
	dij(x,rg,rdis);
	i64 ans=0;
	for(int i=0;i<n;++i)
		ans=max(ans,dis[i]+rdis[i]);
	cout<<ans<<'\n';
	return 0;
}

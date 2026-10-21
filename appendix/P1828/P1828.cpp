//
// 题目：P1828 [USACO3.2] 香甜的黄油 Sweet Butter
// https://www.luogu.com.cn/problem/P1828
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxp{805};
constexpr i64 inf{1145141919810LL};

int n,p,c;
int cnt[mxp];
vector<pair<int,int>>g[mxp];
i64 dis[mxp];
bool vis[mxp];

void dij(int s)
{
	fill_n(dis,p,inf);
	fill_n(vis,p,false);
	priority_queue<pair<i64,int>,vector<pair<i64,int>>,greater<>>Q;
	dis[s]=0;
	Q.emplace(0,s);
	while(!Q.empty())
	{
		auto[dd,u]=Q.top();
		Q.pop();
		if(vis[u])
			continue;
		vis[u]=true;
		for(auto[v,w]:g[u])
			if(dis[v]>dd+w)
			{
				dis[v]=dd+w;
				Q.emplace(dis[v],v);
			}
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>p>>c;
	for(int i=0,x;i<n;++i)
	{
		cin>>x;
		--x;
		++cnt[x];
	}
	for(int i=0,a,b,d;i<c;++i)
	{
		cin>>a>>b>>d;
		--a;--b;
		g[a].push_back({b,d});
		g[b].push_back({a,d});
	}
	i64 ans=inf;
	for(int s=0;s<p;++s)
	{
		dij(s);
		i64 cur=0;
		bool ok=true;
		for(int i=0;i<p;++i)
			if(cnt[i])
			{
				if(dis[i]>=inf)
				{
					ok=false;
					break;
				}
				cur+=cnt[i]*dis[i];
			}
		if(ok)
			ans=min(ans,cur);
	}
	cout<<ans<<'\n';
	return 0;
}

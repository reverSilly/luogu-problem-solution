//
// 题目：P1642 规划
// https://www.luogu.com.cn/problem/P1642
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};
constexpr double inf{1e18};

int n,m,k,w[mxn],c[mxn],sz[mxn];
double val[mxn],dp[mxn][mxn],tmp[mxn];
vector<int>graph[mxn];

void dfs(int u,int fa)
{
	sz[u]=1;
	dp[u][1]=val[u];
	for(int j=2;j<=k;++j)
		dp[u][j]=-inf;

	for(int v:graph[u])
	{
		if(v==fa)
			continue;
		dfs(v,u);
		int lim=min(k,sz[u]+sz[v]);
		for(int j=1;j<=lim;++j)
			tmp[j]=dp[u][j];
		for(int i=1;i<=min(k,sz[u]);++i)
		{
			if(dp[u][i]<-inf/2)
				continue;
			for(int j=1;j<=min(k-i,sz[v]);++j)
				if(dp[v][j]>-inf/2)
					tmp[i+j]=max(tmp[i+j],dp[u][i]+dp[v][j]);
		}
		sz[u]+=sz[v];
		for(int j=1;j<=lim;++j)
			dp[u][j]=tmp[j];
	}
}

bool check(double mid)
{
	for(int i=0;i<n;++i)
		val[i]=w[i]-mid*c[i];
	dfs(0,-1);
	for(int i=0;i<n;++i)
		if(sz[i]>=k&&dp[i][k]>=0)
			return true;
	return false;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	k=n-m;
	for(int i=0;i<n;++i)
		cin>>w[i];
	for(int i=0;i<n;++i)
		cin>>c[i];
	for(int i=1,u,v;i<n;++i)
	{
		cin>>u>>v;--u;--v;
		graph[u].push_back(v);
		graph[v].push_back(u);
	}

	double l=0,r=1e4;
	for(int it=0;it<60;++it)
	{
		double mid=(l+r)/2;
		if(check(mid))
			l=mid;
		else
			r=mid;
	}
	cout<<fixed<<setprecision(1)<<l<<'\n';
	return 0;
}

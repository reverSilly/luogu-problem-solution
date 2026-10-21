//
// 题目：P1807 最长路
// https://www.luogu.com.cn/problem/P1807
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1505};
constexpr i64 ninf{-(i64)1e18};

struct edge
{
	int to,w;
};

int n,m;
vector<edge> g[mxn];
i64 dp[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<m;++i)
	{
		int u,v,w;
		cin>>u>>v>>w;
		--u;
		--v;
		g[u].push_back({v,w});
	}
	fill_n(dp,n,ninf);
	dp[0]=0;
	for(int u=0;u<n;++u)
	{
		if(dp[u]==ninf)
			continue;
		for(edge e:g[u])
			dp[e.to]=max(dp[e.to],dp[u]+e.w);
	}
	if(dp[n-1]==ninf)
		cout<<-1<<'\n';
	else
		cout<<dp[n-1]<<'\n';
	return 0;
}

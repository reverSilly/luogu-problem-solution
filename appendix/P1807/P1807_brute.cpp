//
// 题目：P1807 最长路
// https://www.luogu.com.cn/problem/P1807
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{25};
constexpr i64 ninf{-(i64)1e18};

struct edge
{
	int to,w;
};

int n,m;
vector<edge> g[mxn];
i64 best;
bool reach;

void dfs(int u,i64 dist)
{
	if(u==n-1)
	{
		reach=true;
		best=max(best,dist);
		return;
	}
	for(edge e:g[u])
		dfs(e.to,dist+e.w);
}

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
	best=ninf;
	reach=false;
	dfs(0,0);
	if(!reach)
		cout<<-1<<'\n';
	else
		cout<<best<<'\n';
	return 0;
}

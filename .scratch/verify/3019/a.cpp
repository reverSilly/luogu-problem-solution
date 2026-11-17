// 题目：P3019 [USACO11MAR] Meeting Place S
// 裸 LCA：记忆化求深度，先把深的抬到同深，再同步上爬
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};

int n,m,fa[mxn],dep[mxn];

int getdep(int u)
{
	if(dep[u]>=0)
		return dep[u];
	return dep[u]=getdep(fa[u])+1;	// 父链必达根，递归必终止
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	fill_n(dep,n,-1);
	dep[0]=0;				// 根深度 0；其余 -1 表示未算
	for(int i=1;i<n;++i)
	{
		cin>>fa[i];--fa[i];
	}
	for(int i=0;i<n;++i)
		getdep(i);
	for(int q=0,u,v;q<m;++q)
	{
		cin>>u>>v;--u;--v;
		while(dep[u]>dep[v])
			u=fa[u];
		while(dep[v]>dep[u])
			v=fa[v];
		while(u!=v)
			u=fa[u],v=fa[v];
		cout<<u+1<<'\n';
	}
	return 0;
}

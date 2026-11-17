// 题目：P2941 [USACO09FEB] Surround the Islands S
// 并查集缩出岛屿；岛间最小船费 best[A][B]；枚举母岛，答案 = 2*min(到其余各岛最小费之和)
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{505};
constexpr int inf{1145141919};

int n,f[mxn],c[mxn][mxn],best[mxn][mxn];

int getrt(int x)
{
	if(f[x]==x)
		return x;
	return f[x]=getrt(f[x]);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	iota(f,f+n,0);
	for(int i=0,u,v;i<n;++i)
	{
		cin>>u>>v;--u;--v;
		f[getrt(u)]=getrt(v);
	}
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			cin>>c[i][j];
	fill_n(&best[0][0],mxn*mxn,inf);	// best 是 min 表，必须显式填 INF
	for(int u=0;u<n;++u)
		for(int v=u+1;v<n;++v)
		{
			int ru=getrt(u),rv=getrt(v);
			if(ru!=rv)
			{
				best[ru][rv]=min(best[ru][rv],c[u][v]);
				best[rv][ru]=min(best[rv][ru],c[u][v]);
			}
		}
	int ans=inf;
	for(int i=0;i<n;++i)
		if(getrt(i)==i)	// 枚举母岛（每个连通块的代表元）
		{
			int s=0;
			for(int j=0;j<n;++j)
				if(j!=i&&getrt(j)==j)
					s+=best[i][j];
			ans=min(ans,s);
		}
	cout<<2*ans<<'\n';	// 每次外出必须原路返回，费用翻倍
	return 0;
}

// 题目：P2978 [USACO10JAN] Tea Time S
// 「共同朋友」闭包 = 初始图的连通块（每个连通块最终变成完全图）⇒ 并查集
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};

int n,m,q,f[mxn];

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

	cin>>n>>m>>q;
	iota(f,f+n,0);
	for(int i=0,u,v;i<m;++i)
	{
		cin>>u>>v;--u;--v;
		f[getrt(u)]=getrt(v);
	}
	for(int i=0,x,y;i<q;++i)
	{
		cin>>x>>y;--x;--y;
		cout<<(getrt(x)==getrt(y)?"Y\n":"N\n");
	}
	return 0;
}

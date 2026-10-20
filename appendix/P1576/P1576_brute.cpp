//
// P1576 暴力基准：小图上 DFS 枚举所有简单路径,取"扣除后剩余比例"乘积最大者
// 仅用于 n<=7 的小数据对拍。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxv{10};

int n,m,A,B;
double g[mxv][mxv];   // g[u][v]=该方向扣除手续费后剩余比例(0 表示无边)
bool vis[mxv];
double best;

void dfs(int u,double pr)
{
	if(u==B)
	{
		best=max(best,pr);
		return;
	}
	for(int v=0;v<n;++v)
		if(!vis[v]&&g[u][v]>0.0)
		{
			vis[v]=true;
			dfs(v,pr*g[u][v]);
			vis[v]=false;
		}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>m;
	for(int i=0,x,y,z;i<m;++i)
	{
		cin>>x>>y>>z;--x;--y;
		double f=1.0-z/100.0;
		if(f>g[x][y])
			g[x][y]=f;
		if(f>g[y][x])
			g[y][x]=f;
	}
	cin>>A>>B;--A;--B;
	best=0.0;
	vis[A]=true;
	dfs(A,1.0);
	cout<<fixed<<setprecision(8)<<100.0/best<<'\n';
	return 0;
}

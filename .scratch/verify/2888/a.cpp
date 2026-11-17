// 题目：P2888 [USACO07NOV] Cow Hurdles S
// 瓶颈路（路径上最大边权的最小值）：把 Floyd 的 (+,min) 换成 (max,min)。
// d[i][j]=min(d[i][j],max(d[i][k],d[k][j]))。N<=300 ⇒ O(N^3)=2.7e7 全源预处理，T 个询问 O(1)。
// 陷阱：全局数组默认 0 不是 INF，必须 fill；d[i][i]=0（A==B 不用跨栏）。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{305},inf{1145141919};

int n,m,t,d[mxn][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>t;
	fill_n(&d[0][0],mxn*mxn,inf);
	for(int i=0;i<n;++i)
		d[i][i]=0;
	for(int i=0,u,v,w;i<m;++i)
	{
		cin>>u>>v>>w;--u;--v;
		d[u][v]=min(d[u][v],w);	// 重边取最小栏高
	}
	for(int k=0;k<n;++k)
		for(int i=0;i<n;++i)
			for(int j=0;j<n;++j)
				d[i][j]=min(d[i][j],max(d[i][k],d[k][j]));
	for(int i=0,a,b;i<t;++i)
	{
		cin>>a>>b;--a;--b;
		cout<<(d[a][b]==inf?-1:d[a][b])<<'\n';
	}
	return 0;
}

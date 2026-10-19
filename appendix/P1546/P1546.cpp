//
// 题目：P1546 [USACO3.1] 最短网络 Agri-Net
// https://www.luogu.com.cn/problem/P1546
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};           // N<=100 (+5 松弛)
constexpr int mxm{10005};         // 边数 N*(N-1)/2 <=4950 (+5 松弛)

int n;
int g[mxn][mxn];                  // 邻接矩阵:g[i][j]=农场 i 到 j 的距离
struct{int u,v,w;}e[mxm];         // 边集:(u,v) 与权值 w
int f[mxn];                       // DSU 父指针
int cnt=0;                        // 边数

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
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			cin>>g[i][j];

	for(int i=0;i<n;++i)          // 只取上三角,避免重复边与自环
		for(int j=i+1;j<n;++j)
			e[cnt++]={i,j,g[i][j]};

	sort(e,e+cnt,[](auto a,auto b){return a.w<b.w;});  // 按边权升序
	iota(f,f+n,0);

	int ans=0,used=0;             // used=已合并的边数,达 n-1 即连通
	for(int i=0;i<cnt&&used<n-1;++i)
	{
		int ru=getrt(e[i].u),rv=getrt(e[i].v);
		if(ru!=rv)
		{
			f[ru]=rv;
			ans+=e[i].w;
			++used;
		}
	}
	cout<<ans<<'\n';
	return 0;
}

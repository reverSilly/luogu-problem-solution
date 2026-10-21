//
// 题目：P1613 跑路
// https://www.luogu.com.cn/problem/P1613
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{55},lg{32};        // n<=50, 2^30 足够覆盖 2^31-1
constexpr int inf{1'000'000'000};

int n,m;
bool f[lg][mxn][mxn];                // f[k][i][j]: i 走恰好 2^k 条边可达 j
int dis[mxn][mxn];                   // 以"秒"为单位的最短路

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
	{
		fill_n(dis[i],n,inf);
		dis[i][i]=0;
	}
	for(int i=0,u,v;i<m;++i)
	{
		cin>>u>>v;
		--u;--v;
		f[0][u][v]=true;
		dis[u][v]=min(dis[u][v],1);   // 自环时不要破坏 dis[i][i]=0
	}
	for(int k=1;k<lg;++k)
		for(int i=0;i<n;++i)
			for(int t=0;t<n;++t)
				if(f[k-1][i][t])
					for(int j=0;j<n;++j)
						if(f[k-1][t][j])
							f[k][i][j]=true;
	for(int k=0;k<lg;++k)
		for(int i=0;i<n;++i)
			for(int j=0;j<n;++j)
				if(f[k][i][j])
					dis[i][j]=min(dis[i][j],1);
	for(int k=0;k<n;++k)
		for(int i=0;i<n;++i)
			for(int j=0;j<n;++j)
				if(dis[i][k]+dis[k][j]<dis[i][j])
					dis[i][j]=dis[i][k]+dis[k][j];

	cout<<dis[0][n-1]<<'\n';
	return 0;
}

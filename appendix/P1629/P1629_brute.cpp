//
// 题目：P1629 邮递员送信 （暴力对拍用）
// Floyd 求全源最短路，再累加 1->i 与 i->1
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{105};
constexpr i64 inf{1145141919810LL};

int n,m;
i64 d[mxn][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>m;
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			d[i][j]=i==j?0:inf;
	for(int i=0,u,v;i<m;++i)
	{
		i64 w;
		cin>>u>>v>>w;--u;--v;
		d[u][v]=min(d[u][v],w);
	}
	for(int k=0;k<n;++k)
		for(int i=0;i<n;++i)
			for(int j=0;j<n;++j)
				if(d[i][k]<inf&&d[k][j]<inf)
					d[i][j]=min(d[i][j],d[i][k]+d[k][j]);
	i64 ans=0;
	for(int i=1;i<n;++i)
		ans+=d[0][i]+d[i][0];
	cout<<ans<<'\n';
	return 0;
}

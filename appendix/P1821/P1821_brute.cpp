//
// 题目：P1821 [USACO07FEB] Cow Party S
// https://www.luogu.com.cn/problem/P1821
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{85};
constexpr i64 inf{1145141919810LL};

int n,m,x;
i64 d[mxn][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>m>>x;
	--x;
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			d[i][j]=(i==j?0:inf);
	for(int i=0,u,v,w;i<m;++i)
	{
		cin>>u>>v>>w;
		--u;--v;
		d[u][v]=min(d[u][v],(i64)w);
	}
	for(int k=0;k<n;++k)
		for(int i=0;i<n;++i)
			if(d[i][k]<inf)
				for(int j=0;j<n;++j)
					if(d[k][j]<inf)
						d[i][j]=min(d[i][j],d[i][k]+d[k][j]);
	i64 ans=0;
	for(int i=0;i<n;++i)
		ans=max(ans,d[i][x]+d[x][i]);
	cout<<ans<<'\n';
	return 0;
}

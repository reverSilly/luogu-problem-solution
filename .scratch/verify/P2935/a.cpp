// 题目：P2935 [USACO09JAN] Best Spot S
// P<=500 ⇒ Floyd 求全源最短路 O(P^3)；平均距离最小 ⇔ 距离和最小（F 固定）
// 顺序枚举候选点 + 严格小于更新 ⇒ 平局自动取编号最小
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{505},inf{1145141919};

int p,f,c,dist[mxn][mxn],fav[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>p>>f>>c;
	for(int i=0;i<f;++i)
	{
		cin>>fav[i];--fav[i];
	}
	fill_n(&dist[0][0],mxn*mxn,inf);	// 全局默认 0 不是 INF，必须显式填
	for(int i=0;i<p;++i)
		dist[i][i]=0;
	for(int i=0,u,v,w;i<c;++i)
	{
		cin>>u>>v>>w;--u;--v;
		dist[u][v]=min(dist[u][v],w);	// 重边取最小
		dist[v][u]=min(dist[v][u],w);
	}
	for(int k=0;k<p;++k)
		for(int i=0;i<p;++i)
		{
			if(dist[i][k]==inf)continue;
			for(int j=0;j<p;++j)
				if(dist[k][j]!=inf&&dist[i][k]+dist[k][j]<dist[i][j])
					dist[i][j]=dist[i][k]+dist[k][j];
		}
	i64 best{1145141919810};
	int ans=0;
	for(int i=0;i<p;++i)
	{
		i64 s=0;
		bool ok=true;
		for(int j=0;j<f;++j)
			if(dist[i][fav[j]]==inf){ok=false;break;}
			else s+=dist[i][fav[j]];
		if(ok&&s<best){best=s;ans=i;}
	}
	cout<<ans+1<<'\n';
	return 0;
}
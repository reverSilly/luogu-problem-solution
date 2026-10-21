//
// 题目：P1608 路径统计
// https://www.luogu.com.cn/problem/P1608
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{2005};                  // N<=2000
constexpr int inf{1'000'000'000};

int n,m;
int w[mxn][mxn];                          // 邻接矩阵, 重边取最小
int dis[mxn];                             // 最短路长度
i64 cnt[mxn];                             // 最短路条数
bool vis[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
	{
		fill_n(w[i],n,inf);
		w[i][i]=0;
	}
	for(int i=0,u,v,c;i<m;++i)
	{
		cin>>u>>v>>c;
		--u;--v;
		if(u!=v)
			w[u][v]=min(w[u][v],c);       // 重边只保留最小花费, 否则同一路径会被重复计数
	}
	if(n==1)
	{
		cout<<"0 1\n";
		return 0;
	}
	fill_n(dis,n,inf);
	fill_n(cnt,n,0);
	dis[0]=0;
	cnt[0]=1;
	for(int it=0;it<n;++it)
	{
		int u=-1;
		for(int i=0;i<n;++i)
			if(!vis[i]&&dis[i]<inf&&(u==-1||dis[i]<dis[u]))
				u=i;
		if(u==-1)
			break;
		vis[u]=true;
		for(int v=0;v<n;++v)
		{
			if(vis[v]||v==u||w[u][v]>=inf)
				continue;
			int nd=dis[u]+w[u][v];
			if(nd<dis[v])
			{
				dis[v]=nd;
				cnt[v]=cnt[u];
			}
			else if(nd==dis[v])
				cnt[v]+=cnt[u];
		}
	}
	if(dis[n-1]>=inf)
		cout<<"No answer\n";
	else
		cout<<dis[n-1]<<' '<<cnt[n-1]<<'\n';
	return 0;
}

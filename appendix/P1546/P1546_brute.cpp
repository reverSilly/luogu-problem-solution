// 朴素暴力:稠密 Prim (O(n^2)),与 Kruskal 相互独立
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};
constexpr int inf{1145141919};    // 114514 家族哨兵

int n;
int g[mxn][mxn];
bool vis[mxn];
int dis[mxn];                     // dis[i]=i 到当前生成树的最小边权

int main()
{
	if(!(cin>>n))
		return 0;
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			cin>>g[i][j];

	for(int i=0;i<n;++i)
		dis[i]=inf;
	dis[0]=0;                     // 以农场 0 为根开始
	int ans=0;
	for(int it=0;it<n;++it)
	{
		int u=-1;
		for(int i=0;i<n;++i)
			if(!vis[i]&&(u==-1||dis[i]<dis[u]))
				u=i;
		vis[u]=true;
		ans+=dis[u];
		for(int v=0;v<n;++v)
			if(!vis[v]&&g[u][v]<dis[v])
				dis[v]=g[u][v];
	}
	cout<<ans<<'\n';
	return 0;
}

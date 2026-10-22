#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{2305};
constexpr int inf{0x3f3f3f3f};

int g[mxn][mxn];
int dis[mxn];
bool vis[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n,m;
	cin>>n>>m;
	int tot=n+1;//0 号是 mzc，1~n 号是男家丁
	for(int i=0;i<tot;++i)
		for(int j=0;j<tot;++j)
			g[i][j]=inf;
	for(int i=0;i<m;++i)
	{
		int a,b,c;
		cin>>a>>b>>c;
		//be careful here! 重边按出题人的处理：后读入的直接覆盖旧边权
		g[a][b]=g[b][a]=c;
	}

	//邻接矩阵上的朴素 Prim，O(n^2)
	fill_n(dis,tot,inf);
	dis[0]=0;
	i64 ans=0;
	for(int i=0;i<tot;++i)
	{
		int u=-1;
		for(int j=0;j<tot;++j)
			if(!vis[j]&&(u==-1||dis[j]<dis[u]))
				u=j;
		vis[u]=true;
		ans+=dis[u];
		for(int v=0;v<tot;++v)
			if(!vis[v]&&g[u][v]!=inf&&g[u][v]<dis[v])
				dis[v]=g[u][v];
	}
	cout<<ans<<'\n';
	return 0;
}

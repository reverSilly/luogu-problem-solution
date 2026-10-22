// P2185 暴力：Floyd 求所有点对最短路，取最大值即直径
#include<bits/stdc++.h>
using namespace std;

int g[1005][1005];

int main()
{
	int n,m;
	while(scanf("%d%d",&n,&m)==2)
	{
		if(!n&&!m)
			break;
		for(int i=1;i<=n;++i)
			for(int j=1;j<=n;++j)
				g[i][j]=(i==j?0:1000000000);
		for(int i=1;i<=m;++i)
		{
			int u,v;
			scanf("%d%d",&u,&v);
			// 自环不影响两点间最短路
			if(u!=v)
				g[u][v]=g[v][u]=1;
		}
		for(int k=1;k<=n;++k)
			for(int i=1;i<=n;++i)
				for(int j=1;j<=n;++j)
					if(g[i][k]+g[k][j]<g[i][j])
						g[i][j]=g[i][k]+g[k][j];
		int diam=0;
		for(int i=1;i<=n;++i)
			for(int j=1;j<=n;++j)
				diam=max(diam,g[i][j]);
		printf("%d\n",diam*100);
	}
	return 0;
}

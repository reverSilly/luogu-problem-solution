#include<bits/stdc++.h>
using namespace std;

// 暴力：把格子看成点、跳跃看成边，跑 Bellman-Ford 求最短路（与 BFS 完全不同的算法）
int M,N,M1,M2,g[35][35];
int dx[8],dy[8];
int dis[35][35];

bool can(int x,int y){return x>=0&&x<M&&y>=0&&y<N&&g[x][y]!=0&&g[x][y]!=2;}

int main()
{
	cin>>M>>N>>M1>>M2;
	int sx=0,sy=0,tx=0,ty=0;
	for(int i=0;i<M;++i)
		for(int j=0;j<N;++j)
		{
			char ch;
			cin>>ch;
			g[i][j]=ch-'0';
			if(g[i][j]==3)
			{
				sx=i;sy=j;
			}
			if(g[i][j]==4)
			{
				tx=i;ty=j;
			}
		}
	int ax[8]={M1,M1,-M1,-M1,M2,M2,-M2,-M2};
	int ay[8]={M2,-M2,M2,-M2,M1,-M1,M1,-M1};
	for(int k=0;k<8;++k)
	{
		dx[k]=ax[k];
		dy[k]=ay[k];
	}
	for(int i=0;i<M;++i)
		for(int j=0;j<N;++j)
			dis[i][j]=1e9;
	dis[sx][sy]=0;
	for(int it=0;it<M*N;++it)
		for(int x=0;x<M;++x)
			for(int y=0;y<N;++y)
			{
				if(!can(x,y)||dis[x][y]>=1e9)
					continue;
				for(int k=0;k<8;++k)
				{
					int nx=x+dx[k],ny=y+dy[k];
					if(can(nx,ny)&&dis[x][y]+1<dis[nx][ny])
						dis[nx][ny]=dis[x][y]+1;
				}
			}
	cout<<(dis[tx][ty]>=1e9?-1:dis[tx][ty])<<'\n';
	return 0;
}

#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{35};

int M,N,M1,M2;
int g[mxn][mxn],dis[mxn][mxn];
int qx[mxn*mxn],qy[mxn*mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

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
	int dx[8]={M1,M1,-M1,-M1,M2,M2,-M2,-M2};
	int dy[8]={M2,-M2,M2,-M2,M1,-M1,M1,-M1};
	memset(dis,-1,sizeof(dis));
	int hd=0,tl=0;
	qx[tl]=sx;qy[tl]=sy;++tl;
	dis[sx][sy]=0;
	while(hd<tl)
	{
		int x=qx[hd],y=qy[hd];
		++hd;
		if(x==tx&&y==ty)
			break;
		for(int k=0;k<8;++k)
		{
			int nx=x+dx[k],ny=y+dy[k];
			if(nx<0||nx>=M||ny<0||ny>=N)
				continue;
			if(dis[nx][ny]>=0||g[nx][ny]==0||g[nx][ny]==2)	// 水里、岩石上都不能落脚
				continue;
			dis[nx][ny]=dis[x][y]+1;
			qx[tl]=nx;qy[tl]=ny;++tl;
		}
	}
	cout<<dis[tx][ty]<<'\n';
	return 0;
}

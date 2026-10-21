//
// 题目：P1825 [USACO11OPEN] Corn Maze S
// https://www.luogu.com.cn/problem/P1825
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{40};
constexpr int inf{1145141919};
constexpr int dx[4]{1,-1,0,0};
constexpr int dy[4]{0,0,1,-1};

int n,m,sx,sy,ex,ey;
char g[mxn][mxn];
int dis[mxn][mxn];
int px[26],py[26],qx[26],qy[26],seen[26];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>m;
	for(int i=1;i<=n;++i)
	{
		string s;
		cin>>s;
		for(int j=1;j<=m;++j)
		{
			g[i][j]=s[j-1];
			if(g[i][j]=='@')
				sx=i,sy=j;
			if(g[i][j]=='=')
				ex=i,ey=j;
			if(g[i][j]>='A'&&g[i][j]<='Z')
			{
				int id=g[i][j]-'A';
				if(seen[id]==0)
					px[id]=i,py[id]=j;
				else
					qx[id]=i,qy[id]=j;
				++seen[id];
			}
		}
	}
	for(int i=1;i<=n;++i)
		for(int j=1;j<=m;++j)
			dis[i][j]=inf;
	dis[sx][sy]=0;
	for(int iter=0;iter<n*m;++iter)
	{
		bool chg=false;
		for(int x=1;x<=n;++x)
			for(int y=1;y<=m;++y)
			{
				if(g[x][y]=='#'||dis[x][y]==inf)
					continue;
				for(int k=0;k<4;++k)
				{
					int nx=x+dx[k],ny=y+dy[k];
					if(nx<1||nx>n||ny<1||ny>m||g[nx][ny]=='#')
						continue;
					int tx=nx,ty=ny;
					if(g[nx][ny]>='A'&&g[nx][ny]<='Z')
					{
						int id=g[nx][ny]-'A';
						if(px[id]==nx&&py[id]==ny)
							tx=qx[id],ty=qy[id];
						else
							tx=px[id],ty=py[id];
					}
					if(dis[tx][ty]>dis[x][y]+1)
					{
						dis[tx][ty]=dis[x][y]+1;
						chg=true;
					}
				}
			}
		if(!chg)
			break;
	}
	cout<<dis[ex][ey]<<'\n';
	return 0;
}

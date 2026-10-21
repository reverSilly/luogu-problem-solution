//
// 题目：P1825 [USACO11OPEN] Corn Maze S
// https://www.luogu.com.cn/problem/P1825
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{305};
constexpr int dx[4]{1,-1,0,0};
constexpr int dy[4]{0,0,1,-1};

int n,m;
char g[mxn][mxn];
int dis[mxn][mxn];
int px[26],py[26],qx[26],qy[26],seen[26];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>m;
	int sx=1,sy=1;
	for(int i=1;i<=n;++i)
	{
		string s;
		cin>>s;
		for(int j=1;j<=m;++j)
		{
			g[i][j]=s[j-1];
			if(g[i][j]=='@')
				sx=i,sy=j;
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
	memset(dis,-1,sizeof(dis));
	dis[sx][sy]=0;
	queue<pair<int,int>>Q;
	Q.push({sx,sy});
	while(!Q.empty())
	{
		int x=Q.front().first,y=Q.front().second;
		Q.pop();
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
			if(dis[tx][ty]!=-1)
				continue;
			dis[tx][ty]=dis[x][y]+1;
			if(g[tx][ty]=='=')
			{
				cout<<dis[tx][ty]<<'\n';
				return 0;
			}
			Q.push({tx,ty});
		}
	}
	return 0;
}

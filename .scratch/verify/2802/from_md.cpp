// 题目：P2802 回家
// 状态 (x,y,hp)：同格不同血必须允许重访（死胡同鼠标反例）
// 转移顺序：先判死(hp-1==0 即死) → 再拾取(补满 6) → 再判家
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{14},inf{1145141919};

int n,m,a[mxn][mxn],dis[mxn][mxn][7];
int dx[4]={1,-1,0,0},dy[4]={0,0,1,-1};
struct node{int x,y,hp;}Q[mxn*mxn*7];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	int sx=0,sy=0;
	for(int i=0;i<n;++i)
		for(int j=0;j<m;++j)
		{
			cin>>a[i][j];
			if(a[i][j]==2)
				sx=i,sy=j;
		}
	fill_n(&dis[0][0][0],mxn*mxn*7,inf);	// 全局默认 0 是合法距离，必须显式填 INF
	int qh=0,qt=0;
	dis[sx][sy][6]=0;
	Q[qt++]={sx,sy,6};
	while(qh<qt)
	{
		auto[x,y,hp]=Q[qh++];
		for(int d=0;d<4;++d)
		{
			int nx=x+dx[d],ny=y+dy[d];
			if(nx<0||nx>=n||ny<0||ny>=m||a[nx][ny]==0)
				continue;
			int nhp=hp-1;
			if(nhp==0)	// 血尽即死，鼠标和家都救不回来
				continue;
			if(a[nx][ny]==4)
				nhp=6;	// 鼠标无限次，踏上即满血
			if(a[nx][ny]==3)
			{
				cout<<dis[x][y][hp]+1<<'\n';
				return 0;
			}
			if(dis[nx][ny][nhp]==inf)
			{
				dis[nx][ny][nhp]=dis[x][y][hp]+1;
				Q[qt++]={nx,ny,nhp};
			}
		}
	}
	cout<<-1<<'\n';
	return 0;
}

// P2199 暴力：逐格判断能否看见奖杯（同行/列/对角线且中间无墙），再 BFS 求最短步数
#include<bits/stdc++.h>
using namespace std;

int n,m;
char g[30][30];
int dis[30][30];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		for(int j=0;j<m;++j)cin>>g[i][j];
	int ex,ey,sx,sy;
	while(cin>>ex>>ey>>sx>>sy)
	{
		if(ex==0)break;
		--ex;--ey;--sx;--sy;
		// 逐格判断可见性：与奖杯同行/列/对角线，且中间没有墙
		bool see[30][30]={};
		for(int i=0;i<n;++i)
			for(int j=0;j<m;++j)
			{
				if(i==ex&&j==ey){see[i][j]=true;continue;}
				int dr=i-ex,dc=j-ey;
				if(dr!=0&&dc!=0&&abs(dr)!=abs(dc))continue;
				int si=(ex>i)-(ex<i),sj=(ey>j)-(ey<j);
				int x=i+si,y=j+sj;
				bool ok=true;
				while(x!=ex||y!=ey)
				{
					if(g[x][y]=='X'){ok=false;break;}
					x+=si;y+=sj;
				}
				if(ok)see[i][j]=true;
			}
		// BFS 求哈利到每格的最短步数
		for(int i=0;i<n;++i)
			for(int j=0;j<m;++j)dis[i][j]=-1;
		queue<pair<int,int>>q;
		q.push({sx,sy});
		dis[sx][sy]=0;
		int ans=-1;
		while(!q.empty())
		{
			auto [x,y]=q.front();q.pop();
			if(see[x][y]){ans=dis[x][y];break;}
			const int dx[4]={-1,1,0,0},dy[4]={0,0,-1,1};
			for(int k=0;k<4;++k)
			{
				int tx=x+dx[k],ty=y+dy[k];
				if(tx<0||tx>=n||ty<0||ty>=m)continue;
				if(g[tx][ty]=='X'||dis[tx][ty]!=-1)continue;
				dis[tx][ty]=dis[x][y]+1;
				q.push({tx,ty});
			}
		}
		if(ans<0)cout<<"Poor Harry\n";
		else cout<<ans<<'\n';
	}
	return 0;
}

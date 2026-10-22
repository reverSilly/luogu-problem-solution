// 题目：P2199 最后的迷宫
// 思路：对每次询问，先从奖杯向 8 个方向投射射线，标出所有能看见奖杯的格子
//       再从哈利的位置 BFS（只能上下左右走），取最近的「能看见奖杯」的格子
//       走不到任何这样的格子就输出 Poor Harry
//       N*M<=16384，用一维数组存地图
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{16389};

int n,m;
char g[mxn];
bool see[mxn],vis[mxn];
int dis[mxn],Q[mxn];

const int dx[8]={-1,-1,-1,0,0,1,1,1};
const int dy[8]={-1,0,1,-1,1,-1,0,1};
const int mx[4]={-1,1,0,0};
const int my[4]={0,0,-1,1};

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		for(int j=0;j<m;++j)
			cin>>g[i*m+j];

	int ex,ey,sx,sy;
	while(cin>>ex>>ey>>sx>>sy)
	{
		if(ex==0)break;
		--ex;--ey;--sx;--sy;
		// 奖杯所在的格子当然算「看得见」
		memset(see,0,sizeof(see));
		see[ex*m+ey]=1;
		for(int k=0;k<8;++k)
		{
			int x=ex+dx[k],y=ey+dy[k];
			while(x>=0&&x<n&&y>=0&&y<m&&g[x*m+y]!='X')
			{
				see[x*m+y]=1;
				x+=dx[k];
				y+=dy[k];
			}
		}
		memset(vis,0,sizeof(vis));
		memset(dis,-1,sizeof(dis));
		int hd=0,tl=0;
		Q[tl++]=sx*m+sy;
		vis[sx*m+sy]=1;
		dis[sx*m+sy]=0;
		int ans=-1;
		while(hd<tl)
		{
			int u=Q[hd++];
			if(see[u])
			{
				ans=dis[u];
				break;
			}
			int x=u/m,y=u%m;
			for(int k=0;k<4;++k)
			{
				int tx=x+mx[k],ty=y+my[k];
				if(tx<0||tx>=n||ty<0||ty>=m)continue;
				int v=tx*m+ty;
				if(vis[v]||g[v]=='X')continue;
				vis[v]=1;
				dis[v]=dis[u]+1;
				Q[tl++]=v;
			}
		}
		if(ans<0)cout<<"Poor Harry\n";
		else cout<<ans<<'\n';
	}
	return 0;
}

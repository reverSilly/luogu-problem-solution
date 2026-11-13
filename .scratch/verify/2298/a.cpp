// 题目：P2298 Mzc和男家丁的游戏
// 思路：把非墙格子看成无权图的点，mzc 每次上下左右走一步 = 走一条边
//       从 'm' 出发 BFS，第一次弹出 'd' 时的距离就是最短移动次数
//       弹完队列仍没见到 'd' => 不连通 => No Way!
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{2005};

int n,m,sx,sy;
char g[mxn][mxn];
int dis[mxn][mxn];      // 全局 int 默认 0，必须手动置 -1
int Q[mxn*mxn];         // 4e6 个格子，递归会爆栈，用数组模拟队列

const int dx[4]={-1,1,0,0};
const int dy[4]={0,0,-1,1};

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		for(int j=0;j<m;++j)
		{
			cin>>g[i][j];
			if(g[i][j]=='m')
			{
				sx=i;
				sy=j;
			}
		}

	fill_n(&dis[0][0],mxn*mxn,-1);
	int hd=0,tl=0,ans=-1;
	Q[tl++]=sx*m+sy;
	dis[sx][sy]=0;
	while(hd<tl)
	{
		int u=Q[hd++];
		int x=u/m,y=u%m;
		if(g[x][y]=='d')
		{
			ans=dis[x][y];
			break;
		}
		for(int k=0;k<4;++k)
		{
			int tx=x+dx[k],ty=y+dy[k];
			if(tx<0||tx>=n||ty<0||ty>=m)
				continue;
			if(g[tx][ty]=='#'||dis[tx][ty]!=-1)
				continue;
			dis[tx][ty]=dis[x][y]+1;
			Q[tl++]=tx*m+ty;
		}
	}
	if(ans<0)
		cout<<"No Way!\n";
	else
		cout<<ans<<'\n';
	return 0;
}

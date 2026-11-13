// 题目：P2372 yyy2015c01挑战算周长
// 先从脚踩的格子出发，按 8 连通 BFS 把整个目标标记出来
// 再逐格数「四条边里对面不是同目标格子的那几条」，总和就是这个目标的周长
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{25};

int m,n,X,Y;
char g[mxn][mxn];
bool vis[mxn][mxn];

const int dx[8]={-1,-1,-1,0,0,1,1,1};
const int dy[8]={-1,0,1,-1,1,-1,0,1};
const int ax[4]={-1,1,0,0};
const int ay[4]={0,0,-1,1};

int q[mxn*mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>m>>n>>X>>Y;
	for(int i=0;i<m;++i)
		for(int j=0;j<n;++j)
			cin>>g[i][j];
	--X;--Y;
	int hd=0,tl=0;
	q[tl++]=X*n+Y;
	vis[X][Y]=1;
	while(hd<tl)
	{
		int u=q[hd++],x=u/n,y=u%n;
		for(int k=0;k<8;++k)
		{
			int tx=x+dx[k],ty=y+dy[k];
			if(tx<0||tx>=m||ty<0||ty>=n)continue;
			if(vis[tx][ty]||g[tx][ty]!='X')continue;
			vis[tx][ty]=1;
			q[tl++]=tx*n+ty;
		}
	}
	//目标已标记完，现在逐格数「不与任何同目标格子共享」的边
	int ans=0;
	for(int i=0;i<m;++i)
		for(int j=0;j<n;++j)
		{
			if(!vis[i][j])continue;
			for(int k=0;k<4;++k)
			{
				int ti=i+ax[k],tj=j+ay[k];
				if(ti<0||ti>=m||tj<0||tj>=n||!vis[ti][tj])
					++ans;
			}
		}
	cout<<ans<<'\n';
	return 0;
}

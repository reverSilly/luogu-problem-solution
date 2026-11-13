// 题目：P2385 [USACO07FEB] Bronze Lilypad Pond B
// 思路：每步代价 1 ⇒ 无权最短路 ⇒ 在格点图上 BFS。
//       8 个跳跃偏移是 (±M1,±M2) 与 (±M2,±M1)；
//       只能落在 1/3/4 上（水 0、岩石 2 都不行）。
//       用 char 逐字符读入，兼容 "0101" 与 "0 1 0 1" 两种格式。
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
			cin>>ch;					// 自动跳过空白，连写/空格分隔都行
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
	memset(dis,-1,sizeof(dis));		// 全局 int 默认 0，必须显式置 -1
	int hd=0,tl=0;
	qx[tl]=sx;qy[tl]=sy;++tl;
	dis[sx][sy]=0;
	while(hd<tl)
	{
		int x=qx[hd],y=qy[hd];
		++hd;
		if(x==tx&&y==ty)			// 出队即最短
			break;
		for(int k=0;k<8;++k)
		{
			int nx=x+dx[k],ny=y+dy[k];
			if(nx<0||nx>=M||ny<0||ny>=N)
				continue;
			if(dis[nx][ny]>=0||g[nx][ny]==0||g[nx][ny]==2)	// 已访问 / 水 / 岩石
				continue;
			dis[nx][ny]=dis[x][y]+1;
			qx[tl]=nx;qy[tl]=ny;++tl;
		}
	}
	cout<<dis[tx][ty]<<'\n';
	return 0;
}

// 题目：P2937 [USACO09JAN] Laserphones S
// 状态 (格,方向)：直行边权 0，转弯(放镜子)边权 1 ⇒ 0-1 BFS
// 方向 上0 右1 下2 左3，垂直方向 = d^1 与 d^3；答案 = min_d dist[终点][d]
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105},inf{1145141919};
constexpr int dr[4]{-1,0,1,0},dc[4]{0,1,0,-1};

int w,h,dist[mxn][mxn][4];
char g[mxn][mxn];
bool done[mxn][mxn][4];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>w>>h;
	int sr=-1,sc=-1,tr=-1,tc=-1;
	for(int r=0;r<h;++r)
	{
		cin>>g[r];
		for(int c=0;c<w;++c)
			if(g[r][c]=='C')
			{
				if(sr<0)sr=r,sc=c;
				else tr=r,tc=c;
			}
	}
	fill_n(&dist[0][0][0],mxn*mxn*4,inf);
	deque<array<int,3>>Q;
	for(int d=0;d<4;++d)
	{
		dist[sr][sc][d]=0;	// 起点向任意方向免费发射激光
		Q.push_front({sr,sc,d});
	}
	while(!Q.empty())
	{
		auto[r,c,d]=Q.front();Q.pop_front();
		if(done[r][c][d])continue;
		done[r][c][d]=true;
		int cur=dist[r][c][d];
		int nr=r+dr[d],nc=c+dc[d];
		if(nr>=0&&nr<h&&nc>=0&&nc<w&&g[nr][nc]!='*'&&cur<dist[nr][nc][d])
		{
			dist[nr][nc][d]=cur;	// 直行 0 代价，入队首
			Q.push_front({nr,nc,d});
		}
		for(int nd:{d^1,d^3})
			if(cur+1<dist[r][c][nd])
			{
				dist[r][c][nd]=cur+1;	// 原地放镜子转 90°，1 代价，入队尾
				Q.push_back({r,c,nd});
			}
	}
	int ans=inf;
	for(int d=0;d<4;++d)
		ans=min(ans,dist[tr][tc][d]);
	cout<<ans<<'\n';
	return 0;
}

//
// 题目：P1605 迷宫
// https://www.luogu.com.cn/problem/P1605
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{10};            // N,M<=5 (+5 松弛)

int n,m,t,sx,sy,fx,fy,ans;
bool block[mxn][mxn],vis[mxn][mxn];
constexpr int dx[4]{1,-1,0,0};
constexpr int dy[4]{0,0,1,-1};

void dfs(int x,int y)
{
	if(x==fx&&y==fy)              // 到达终点即得一种方案, 不再从终点继续扩展
	{
		++ans;
		return;
	}
	for(int k=0;k<4;++k)
	{
		int nx=x+dx[k],ny=y+dy[k];
		if(nx<1||nx>n||ny<1||ny>m||block[nx][ny]||vis[nx][ny])
			continue;
		vis[nx][ny]=true;
		dfs(nx,ny);
		vis[nx][ny]=false;
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>t>>sx>>sy>>fx>>fy;
	for(int i=0,x,y;i<t;++i)
	{
		cin>>x>>y;
		block[x][y]=true;
	}

	vis[sx][sy]=true;             // 每个格子最多经过一次, 起点先打标记
	dfs(sx,sy);
	cout<<ans<<'\n';
	return 0;
}

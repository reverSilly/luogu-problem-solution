//
// P1605 暴力: 枚举从起点出发的所有自回避路径, 统计"终点恰为终点格"的路径数
// 与标程不同: 到达终点后不截断, 继续扩展; 靠路径末格是否为终点来计数
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{10};

int n,m,t,fx,fy,ans;
bool block[mxn][mxn],vis[mxn][mxn];
constexpr int dx[4]{1,-1,0,0};
constexpr int dy[4]{0,0,1,-1};

void dfs(int x,int y)
{
	if(x==fx&&y==fy)
		++ans;                       // 计数后继续扩展, 检验"经过终点再离开"不被重复计数
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

	int sx,sy;
	cin>>n>>m>>t>>sx>>sy>>fx>>fy;
	for(int i=0,x,y;i<t;++i)
	{
		cin>>x>>y;
		block[x][y]=true;
	}
	vis[sx][sy]=true;
	dfs(sx,sy);
	cout<<ans<<'\n';
	return 0;
}

// 题目：P2960 [USACO09OCT] Invasion of the Milkweed G
// 八方向 BFS：第 t 周被占领 <=> 距源点图上距离 <= t；答案 = 最大距离
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105},inf{1145141919};
constexpr int dx[8]{-1,-1,-1,0,0,1,1,1},dy[8]{-1,0,1,-1,1,-1,0,1};

int x,y,mx,my,dis[mxn][mxn],ans;
char g[mxn][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>x>>y>>mx>>my;
	for(int i=0;i<y;++i)
		cin>>g[i];
	fill_n(&dis[0][0],mxn*mxn,inf);	// 全局默认 0 会被当成"已访问"，必须置 INF
	int sr=y-my,sc=mx-1;	// (1,1) 在左下角：坐标行 my = 自上而下第 y-my 行（0-based）
	queue<int>Q;
	dis[sr][sc]=0;
	Q.push(sr*mxn+sc);
	while(!Q.empty())
	{
		int u=Q.front();Q.pop();
		int ux=u/mxn,uy=u%mxn;
		ans=max(ans,dis[ux][uy]);
		for(int d=0;d<8;++d)
		{
			int nx=ux+dx[d],ny=uy+dy[d];
			if(nx<0||nx>=y||ny<0||ny>=x||g[nx][ny]=='*'||dis[nx][ny]!=inf)
				continue;
			dis[nx][ny]=dis[ux][uy]+1;
			Q.push(nx*mxn+ny);
		}
	}
	cout<<ans<<'\n';
	return 0;
}
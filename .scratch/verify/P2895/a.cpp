// 题目：P2895 Meteor Shower S
// dgr[x][y]=该格被烧毁的最早时刻（流星砸自己+四邻），inf=永远安全
// BFS：到达时刻 t 必须严格 <dgr；每格只在最早到达时刻入队一次
// 流星坐标<=300 ⇒ 焦土<=301 ⇒ 坐标 302 必安全，搜索框 [0,302] 足够
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{305},lim{302},inf{1145141919};

int m,dgr[mxn][mxn],dis[mxn][mxn];
int dx[4]={1,-1,0,0},dy[4]={0,0,1,-1};
queue<pair<int,int>>Q;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	fill_n(&dgr[0][0],mxn*mxn,inf);	// 全局默认 0 不是 inf！
	fill_n(&dis[0][0],mxn*mxn,inf);	// dis=inf 兼作"未访问"
	cin>>m;
	for(int i=0,x,y,t;i<m;++i)
	{
		cin>>x>>y>>t;
		dgr[x][y]=min(dgr[x][y],t);
		for(int k=0;k<4;++k)
		{
			int nx=x+dx[k],ny=y+dy[k];
			if(nx>=0&&ny>=0)
				dgr[nx][ny]=min(dgr[nx][ny],t);
		}
	}
	if(dgr[0][0]==0)	// 时刻 0 原点已毁，贝茜无法出发
	{
		cout<<-1<<'\n';
		return 0;
	}
	dis[0][0]=0;
	Q.push({0,0});
	while(!Q.empty())
	{
		auto[x,y]=Q.front();Q.pop();
		int t=dis[x][y];
		if(dgr[x][y]==inf)	// BFS 按时间出队，首个安全格即答案
		{
			cout<<t<<'\n';
			return 0;
		}
		for(int k=0;k<4;++k)
		{
			int nx=x+dx[k],ny=y+dy[k];
			if(nx<0||ny<0||nx>lim||ny>lim)
				continue;
			if(dis[nx][ny]!=inf||t+1>=dgr[nx][ny])
				continue;	// 必须严格早于烧毁时刻
			dis[nx][ny]=t+1;
			Q.push({nx,ny});
		}
	}
	cout<<-1<<'\n';
	return 0;
}
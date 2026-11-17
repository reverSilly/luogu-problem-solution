// 题目：P2873 [USACO07DEC] Mud Puddles S
// 点障碍网格最短路 ⇒ BFS；坐标平移 +501，范围 [-501,501] 足够（夹逼引理）
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1010},off{501},lim{1003};	// 合法下标 [0,1002] ↔ 坐标 [-501,501]
constexpr int inf{1145141919};

int X,Y,n,dis[mxn][mxn];
bool mud[mxn][mxn];
int dx[4]{1,-1,0,0},dy[4]{0,0,1,-1};

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>X>>Y>>n;
	X+=off;Y+=off;
	for(int i=0,a,b;i<n;++i)
	{
		cin>>a>>b;
		mud[a+off][b+off]=true;
	}
	fill_n(&dis[0][0],mxn*mxn,inf);	// 全局 int 默认 0 不是 INF！
	queue<pair<int,int>>Q;
	dis[off][off]=0;
	Q.push({off,off});
	while(!Q.empty())
	{
		auto[x,y]=Q.front();Q.pop();
		for(int d=0;d<4;++d)
		{
			int nx=x+dx[d],ny=y+dy[d];
			if(nx<0||nx>=lim||ny<0||ny>=lim||mud[nx][ny]||dis[nx][ny]!=inf)
				continue;
			dis[nx][ny]=dis[x][y]+1;
			Q.push({nx,ny});
		}
	}
	cout<<dis[X][Y]<<'\n';
	return 0;
}

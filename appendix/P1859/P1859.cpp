//
// 题目：P1859 不听话的机器人
// https://www.luogu.com.cn/problem/P1859
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxm{105};
constexpr int mxn{1005};
constexpr int inf{1145141919};
constexpr int dx[4]{-1,0,1,0};
constexpr int dy[4]{0,1,0,-1};

int m,n,sx,sy;
char g[mxm][mxm];
int cmd[mxn];
int dp[2][mxm][mxm][4];

bool inside(int x,int y)
{
	return x>=1&&x<=m&&y>=1&&y<=m&&g[x][y]=='.';
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>m>>n>>sx>>sy;
	for(int i=1;i<=m;++i)
		for(int j=1;j<=m;++j)
			cin>>g[i][j];
	for(int i=0;i<n;++i)
	{
		string s;
		cin>>s;
		if(s=="FORWARD")
			cmd[i]=0;
		else if(s=="BACK")
			cmd[i]=1;
		else if(s=="LEFT")
			cmd[i]=2;
		else
			cmd[i]=3;
	}

	for(int x=1;x<=m;++x)
		for(int y=1;y<=m;++y)
			for(int d=0;d<4;++d)
				dp[0][x][y][d]=inf;
	dp[0][sx][sy][0]=0;

	int cur=0;
	for(int i=0;i<n;++i)
	{
		int nxt=cur^1;
		for(int x=1;x<=m;++x)
			for(int y=1;y<=m;++y)
				for(int d=0;d<4;++d)
					dp[nxt][x][y][d]=inf;
		for(int x=1;x<=m;++x)
			for(int y=1;y<=m;++y)
				for(int d=0;d<4;++d)
				{
					if(dp[cur][x][y][d]==inf)
						continue;
					int base=dp[cur][x][y][d];
					dp[nxt][x][y][d]=min(dp[nxt][x][y][d],base+1);
					if(cmd[i]==2)
					{
						int nd=(d+3)&3;
						dp[nxt][x][y][nd]=min(dp[nxt][x][y][nd],base);
					}
					else if(cmd[i]==3)
					{
						int nd=(d+1)&3;
						dp[nxt][x][y][nd]=min(dp[nxt][x][y][nd],base);
					}
					else
					{
						int sgn=cmd[i]==0?1:-1;
						int nx=x+sgn*dx[d];
						int ny=y+sgn*dy[d];
						if(inside(nx,ny))
							dp[nxt][nx][ny][d]=min(dp[nxt][nx][ny][d],base);
					}
				}
		cur=nxt;
	}

	int ans=inf;
	for(int x=1;x<=m;++x)
		for(int y=1;y<=m;++y)
			for(int d=0;d<4;++d)
				ans=min(ans,dp[cur][x][y][d]);
	cout<<ans<<'\n';
	return 0;
}

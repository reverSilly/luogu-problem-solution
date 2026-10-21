//
// 题目：P1859 不听话的机器人
// https://www.luogu.com.cn/problem/P1859
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxm{25};
constexpr int mxn{25};
constexpr int dx[4]{-1,0,1,0};
constexpr int dy[4]{0,1,0,-1};

int m,n,sx,sy;
char g[mxm][mxm];
int cmd[mxn];
int best;

bool inside(int x,int y)
{
	return x>=1&&x<=m&&y>=1&&y<=m&&g[x][y]=='.';
}

// 按指令顺序深搜：拒绝或尝试执行。与标程分层 DP 不同
void dfs(int i,int x,int y,int d,int rej)
{
	if(rej>=best)
		return;
	if(i==n)
	{
		best=rej;
		return;
	}
	dfs(i+1,x,y,d,rej+1);
	if(cmd[i]==2)
		dfs(i+1,x,y,(d+3)&3,rej);
	else if(cmd[i]==3)
		dfs(i+1,x,y,(d+1)&3,rej);
	else
	{
		int sgn=cmd[i]==0?1:-1;
		int nx=x+sgn*dx[d];
		int ny=y+sgn*dy[d];
		if(inside(nx,ny))
			dfs(i+1,nx,ny,d,rej);
	}
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
	best=n;
	dfs(0,sx,sy,0,0);
	cout<<best<<'\n';
	return 0;
}

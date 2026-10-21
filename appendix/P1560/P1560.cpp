//
// 题目：P1560 [USACO5.2] 蜗牛的旅行Snail Trails
// https://www.luogu.com.cn/problem/P1560
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{125};           // N<=120 (+5 松弛)
constexpr int dr[4]{1,-1,0,0};
constexpr int dc[4]{0,0,1,-1};

int n,b,ans;
bool obs[mxn][mxn],vis[mxn][mxn];

bool in_bounds(int r,int c){return r>=0&&r<n&&c>=0&&c<n;}

void dfs(int r,int c,int d,int cnt)
{
	int nr=r+dr[d],nc=c+dc[d];
	int steps=0;
	while(in_bounds(nr,nc)&&!obs[nr][nc]&&!vis[nr][nc])
	{
		vis[nr][nc]=true;
		r=nr;c=nc;
		++steps;
		nr+=dr[d];nc+=dc[d];
	}
	if(steps==0)
		return;
	ans=max(ans,cnt+steps);

	// 只有撞上棋盘边界或路障 '#' 时才允许转弯 90 度; 若撞上自身已走轨迹则必须停止
	if(!in_bounds(nr,nc)||obs[nr][nc])
	{
		int start_nd=(d<2)?2:0;
		for(int nd=start_nd;nd<start_nd+2;++nd)
		{
			int tr=r+dr[nd],tc=c+dc[nd];
			if(in_bounds(tr,tc)&&!obs[tr][tc]&&!vis[tr][tc])
				dfs(r,c,nd,cnt+steps);
		}
	}

	// 回溯撤销本段滑行标记
	for(int k=0;k<steps;++k)
	{
		vis[r][c]=false;
		r-=dr[d];c-=dc[d];
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n>>b))
		return 0;
	for(int i=0;i<b;++i)
	{
		string s;
		cin>>s;
		int col=static_cast<unsigned char>(s[0])-'A';
		int row=0;
		for(size_t j=1;j<s.size();++j)
			if(isdigit(static_cast<unsigned char>(s[j])))
				row=row*10+(s[j]-'0');
		--row;
		if(in_bounds(row,col))
			obs[row][col]=true;
	}

	vis[0][0]=true;
	ans=1;
	dfs(0,0,0,1);                 // 初始向下
	dfs(0,0,2,1);                 // 初始向右
	cout<<ans<<'\n';
	return 0;
}

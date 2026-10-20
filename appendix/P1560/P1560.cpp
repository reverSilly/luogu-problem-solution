//
// 题目：P1560 [USACO5.2] 蜗牛的旅行Snail Trails
// https://www.luogu.com.cn/problem/P1560
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{130};           // N<=120,外加一圈边界 (+5 松弛)

int N,B;
int bar[mxn][mxn];                // 1=路障或棋盘外边界(边界一律当路障)
bool vis[mxn][mxn];               // 已经过的格子
int ans;

void dfs(int x,int y,int len)     // 当前在 (x,y),已访问 len 个格子(含起点)
{
	if(len>ans)
		ans=len;
	for(int d=0;d<4;++d)              // 4 个方向都试;前进/后退方向会分别被跳掉/挡死
	{
		int dx=d==0?-1:(d==1?1:0);
		int dy=d==2?-1:(d==3?1:0);
		int t=x+dx,s=y+dy;
		if(bar[t][s])                 // 该方向第一格就是路障/边界,不能起步
			continue;
		int z=len,cnt=0;
		bool hitvis=false;
		while(!bar[t][s])             // 一路直行,直到路障/边界
		{
			if(vis[t][s])             // 撞上已访问格:本次散步到此为止,不再转弯
			{
				if(z>ans)
					ans=z;
				hitvis=true;
				break;
			}
			vis[t][s]=true;
			++z;
			++cnt;
			t+=dx;
			s+=dy;
		}
		if(!hitvis)                   // 停在路障前,再在下一层里左转/右转
			dfs(t-dx,s-dy,z);
		int bx=x+dx,by=y+dy;          // 回滚本段占用的格子
		for(int k=0,tx=bx,ty=by;k<cnt;++k,tx+=dx,ty+=dy)
			vis[tx][ty]=false;
	}
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>N>>B;
	for(int i=0;i<=N+1;++i)
	{
		bar[0][i]=bar[N+1][i]=1;
		bar[i][0]=bar[i][N+1]=1;
	}
	for(int i=0;i<B;++i)
	{
		string t;
		cin>>t;                        // 形如 E2:字母为列,数字为行
		int c=t[0]-'A'+1;
		int r=0;
		for(int j=1;j<(int)t.size();++j)
			r=r*10+(t[j]-'0');
		bar[r][c]=1;
	}
	vis[1][1]=true;
	dfs(1,1,1);
	cout<<ans<<'\n';
	return 0;
}

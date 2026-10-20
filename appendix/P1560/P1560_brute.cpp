#include<bits/stdc++.h>
using namespace std;

// 小数据朴素基准:逐格模拟。
// 规则:能直行就必须直行;前方撞到边界/路障才允许左转或右转;
//       前方是已访问格则本分支结束(不再转弯)。
constexpr int mxn{40};

int N,B;
int bar[mxn][mxn];
bool vis[mxn][mxn];
int ans;
int dx[4]={-1,1,0,0},dy[4]={0,0,-1,1};

void go(int x,int y,int dir,int len)
{
	if(len>ans)
		ans=len;
	int nx=x+dx[dir],ny=y+dy[dir];
	bool inb=(nx>=1&&nx<=N&&ny>=1&&ny<=N);
	if(inb&&!bar[nx][ny]&&!vis[nx][ny])   // 前方可走:必须直行
	{
		vis[nx][ny]=true;
		go(nx,ny,dir,len+1);
		vis[nx][ny]=false;
		return;
	}
	if(!inb||bar[nx][ny])                 // 撞到边界/路障:转 90°
	{
		int d0=dir<2?2:0,d1=dir<2?4:2;
		for(int d=d0;d<d1;++d)
		{
			int mx=x+dx[d],my=y+dy[d];
			if(mx>=1&&mx<=N&&my>=1&&my<=N&&!bar[mx][my]&&!vis[mx][my])
			{
				vis[mx][my]=true;
				go(mx,my,d,len+1);
				vis[mx][my]=false;
			}
		}
	}
	// 前方是已访问格 -> 停止,不转弯
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>N>>B;
	for(int i=0;i<B;++i)
	{
		string t;
		cin>>t;
		int c=t[0]-'A'+1;
		int r=0;
		for(int j=1;j<(int)t.size();++j)
			r=r*10+(t[j]-'0');
		bar[r][c]=1;
	}
	vis[1][1]=true;
	ans=1;
	int d0[2]={1,3};                      // 起点只能向下或向右
	for(int k=0;k<2;++k)
	{
		int d=d0[k],mx=1+dx[d],my=1+dy[d];
		if(mx>=1&&mx<=N&&my>=1&&my<=N&&!bar[mx][my]&&!vis[mx][my])
		{
			vis[mx][my]=true;
			go(mx,my,d,2);
			vis[mx][my]=false;
		}
	}
	cout<<ans<<'\n';
	return 0;
}

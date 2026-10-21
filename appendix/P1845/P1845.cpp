//
// 题目：P1845 影像之结构化特征
// https://www.luogu.com.cn/problem/P1845
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};
constexpr int dx[4]{-1,1,0,0};
constexpr int dy[4]{0,0,-1,1};

int n,g[mxn][mxn],vis[mxn][mxn];
int qx[mxn*mxn],qy[mxn*mxn];
int stepv[mxn*mxn];

int bfs(int sx,int sy)
{
	int h=0,t=0;
	qx[t]=sx;
	qy[t]=sy;
	++t;
	vis[sx][sy]=1;
	int best=1;
	while(h<t)
	{
		int x=qx[h],y=qy[h];
		++h;
		int d=vis[x][y];
		if(d>best)
			best=d;
		for(int k=0;k<4;++k)
		{
			int nx=x+dx[k],ny=y+dy[k];
			if(nx<0||ny<0||nx>=n||ny>=n)
				continue;
			if(!g[nx][ny]||vis[nx][ny])
				continue;
			vis[nx][ny]=d+1;
			qx[t]=nx;
			qy[t]=ny;
			++t;
		}
	}
	return best;
}

void solve()
{
	memset(g,0,sizeof(g));
	memset(vis,0,sizeof(vis));
	string line;
	getline(cin,line);
	for(int i=0;i<n;++i)
	{
		getline(cin,line);
		int j=0;
		for(char c:line)
		{
			if(c=='0'||c=='1')
			{
				if(j<n)
					g[i][j++]=c-'0';
			}
		}
	}
	int cnt=0;
	for(int i=0;i<n;++i)
	{
		for(int j=0;j<n;++j)
		{
			if(g[i][j]&&!vis[i][j])
				stepv[cnt++]=bfs(i,j);
		}
	}
	sort(stepv,stepv+cnt);
	cout<<cnt<<'\n';
	for(int i=0;i<cnt;++i)
		cout<<stepv[i]<<'\n';
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	while(cin>>n)
		solve();
	return 0;
}

// P1519 暴力基准：对每个格子单独 BFS 找最近出口, 再取最大
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{210},mxm{90};
int W,H,R,C;
char g[mxn][mxm];
bool vis[mxn][mxm];
int qr[mxn*mxm],qc[mxn*mxm];

bool isExitAdj(int r,int c)            // 该格能否一步走出迷宫
{
	if(r==1&&g[0][c]==' ')
		return true;
	if(r==R-2&&g[R-1][c]==' ')
		return true;
	if(c==1&&g[r][0]==' ')
		return true;
	if(c==C-2&&g[r][C-1]==' ')
		return true;
	return false;
}

int bfs(int sr,int sc)                 // 从 (sr,sc) 出发走出迷宫的最少步数
{
	memset(vis,0,sizeof(vis));
	int hd=0,tl=0,d=0;
	qr[tl]=sr;qc[tl]=sc;++tl;vis[sr][sc]=true;
	while(hd<tl)
	{
		int lvl=tl;
		for(;hd<lvl;++hd)
		{
			int r=qr[hd],c=qc[hd];
			if(isExitAdj(r,c))
				return d+1;            // 再走一步出迷宫
			if(r>=2&&g[r-1][c]==' '&&!vis[r-2][c]){vis[r-2][c]=true;qr[tl]=r-2;qc[tl]=c;++tl;}
			if(r+2<=R-2&&g[r+1][c]==' '&&!vis[r+2][c]){vis[r+2][c]=true;qr[tl]=r+2;qc[tl]=c;++tl;}
			if(c>=2&&g[r][c-1]==' '&&!vis[r][c-2]){vis[r][c-2]=true;qr[tl]=r;qc[tl]=c-2;++tl;}
			if(c+2<=C-2&&g[r][c+1]==' '&&!vis[r][c+2]){vis[r][c+2]=true;qr[tl]=r;qc[tl]=c+2;++tl;}
		}
		++d;
	}
	return 1<<30;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>W>>H;
	R=2*H+1;C=2*W+1;
	{
		string tmp;
		getline(cin,tmp);
	}
	for(int i=0;i<R;++i)
	{
		string l;
		getline(cin,l);
		while((int)l.size()<C)
			l+=' ';
		for(int j=0;j<C;++j)
			g[i][j]=l[j];
	}
	int ans=0;
	for(int r=1;r<=R-2;r+=2)
		for(int c=1;c<=C-2;c+=2)
			ans=max(ans,bfs(r,c));
	cout<<ans<<'\n';
	return 0;
}

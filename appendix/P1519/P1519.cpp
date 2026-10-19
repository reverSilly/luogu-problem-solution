// 题目：P1519 [USACO2.4] 穿越栅栏 Overfencing
// https://www.luogu.com.cn/problem/P1519
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{210};                // 2*100+1 = 201 (+5 松弛)
constexpr int mxm{90};                 // 2*38+1  = 77  (+5 松弛)

int W,H,R,C;
char g[mxn][mxm];
int dis[mxn][mxm];                     // 当前格走出迷宫所需步数, -1=未访问
int qr[mxn*mxm],qc[mxn*mxm];
int hd,tl;

void push(int r,int c,int d)           // 未访问过才入队
{
	if(dis[r][c]<0)
	{
		dis[r][c]=d;
		qr[tl]=r;qc[tl]=c;++tl;
	}
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
		getline(cin,tmp);              // 吃掉第一行行末
	}
	for(int i=0;i<R;++i)
	{
		string l;
		getline(cin,l);
		while((int)l.size()<C)
			l+=' ';                    // 行尾空白可能被截断,右侧补空格
		for(int j=0;j<C;++j)
			g[i][j]=l[j];
	}
	memset(dis,-1,sizeof(int)*mxn*mxm);

	// 四段边界上的空格即出口, 其内侧格子一步即可走出
	for(int c=1;c<C-1;c+=2)
	{
		if(g[0][c]==' ')
			push(1,c,1);
		if(g[R-1][c]==' ')
			push(R-2,c,1);
	}
	for(int r=1;r<R-1;r+=2)
	{
		if(g[r][0]==' ')
			push(r,1,1);
		if(g[r][C-1]==' ')
			push(r,C-2,1);
	}

	while(hd<tl)                       // 多源 BFS: 距离 = 到最近出口步数
	{
		int r=qr[hd],c=qc[hd];++hd;
		int d=dis[r][c];
		if(r>=2&&g[r-1][c]==' ')
			push(r-2,c,d+1);
		if(r+2<=R-2&&g[r+1][c]==' ')
			push(r+2,c,d+1);
		if(c>=2&&g[r][c-1]==' ')
			push(r,c-2,d+1);
		if(c+2<=C-2&&g[r][c+1]==' ')
			push(r,c+2,d+1);
	}

	int ans=0;
	for(int r=1;r<=R-2;r+=2)
		for(int c=1;c<=C-2;c+=2)
			ans=max(ans,dis[r][c]);    // 最"糟糕"的点
	cout<<ans<<'\n';
	return 0;
}

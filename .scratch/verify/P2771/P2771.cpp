// 题目：P2771 [USACO16JAN] Build Gates S
// 围栏把平面分成 R 个区域，答案 = R-1（区域邻接图连通，生成树需 R-1 条边=门）
// 区域 = 网格细胞在“无围栏边”上的连通块；flood fill 计数 R
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{2010};

int n,off,dim;
bool h[mxn][mxn],v[mxn][mxn],vis[mxn][mxn];
string s;
int q[mxn*mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>s;
	off=n+2;dim=2*n+5;	// 坐标 ∈[-n,n]，平移 off，留 2 格边界保证外部连通
	int x=0,y=0;
	for(int i=1;i<=n;++i)
	{
		int nx=x,ny=y;
		if(s[i-1]=='N')++ny;
		else if(s[i-1]=='S')--ny;
		else if(s[i-1]=='E')++nx;
		else --nx;
		if(nx==x)v[x+off][min(y,ny)+off]=true;	// 竖直边 (x,y)-(x,y±1)
		else h[min(x,nx)+off][y+off]=true;	// 水平边 (x,y)-(x±1,y)
		x=nx;y=ny;
	}
	int R=0;
	for(int I=0;I<dim;++I)
		for(int J=0;J<dim;++J)
			if(!vis[I][J])
			{
				++R;
				int head=0,tail=0;
				q[tail++]=I*dim+J;vis[I][J]=true;
				while(head<tail)
				{
					int cur=q[head++],ci=cur/dim,cj=cur%dim;
					// 右邻：共享竖直边 v[ci+1][cj]
					if(ci+1<dim&&!vis[ci+1][cj]&&!v[ci+1][cj]){vis[ci+1][cj]=true;q[tail++]=(ci+1)*dim+cj;}
					// 左邻：共享竖直边 v[ci][cj]
					if(ci>0&&!vis[ci-1][cj]&&!v[ci][cj]){vis[ci-1][cj]=true;q[tail++]=(ci-1)*dim+cj;}
					// 上邻：共享水平边 h[ci][cj+1]
					if(cj+1<dim&&!vis[ci][cj+1]&&!h[ci][cj+1]){vis[ci][cj+1]=true;q[tail++]=ci*dim+cj+1;}
					// 下邻：共享水平边 h[ci][cj]
					if(cj>0&&!vis[ci][cj-1]&&!h[ci][cj]){vis[ci][cj-1]=true;q[tail++]=ci*dim+cj-1;}
				}
			}
	cout<<R-1<<'\n';
	return 0;
}
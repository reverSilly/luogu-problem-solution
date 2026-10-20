//
// 题目：P1535 [USACO08MAR] Cow Travelling S
// https://www.luogu.com.cn/problem/P1535
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{105};           // 行列 <=100 (+5 松弛)

int n,m,t;
char g[mxn][mxn];
i64 cur[mxn][mxn],nxt[mxn][mxn];
int dx[4]{-1,1,0,0},dy[4]{0,0,-1,1};

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>t;
	for(int i=0;i<n;++i)
		cin>>g[i];
	int r1,c1,r2,c2;
	cin>>r1>>c1>>r2>>c2;
	--r1;--c1;--r2;--c2;

	cur[r1][c1]=1;                 // 第 0 秒只在起点,方案数 1
	for(int s=0;s<t;++s)
	{
		for(int i=0;i<n;++i)
			for(int j=0;j<m;++j)
				nxt[i][j]=0;       // 换一秒必须清零,避免残留
		for(int i=0;i<n;++i)
			for(int j=0;j<m;++j)
			{
				if(!cur[i][j])
					continue;
				for(int d=0;d<4;++d)
				{
					int ni=i+dx[d],nj=j+dy[d];
					if(ni<0||ni>=n||nj<0||nj>=m||g[ni][nj]=='*')
						continue;
					nxt[ni][nj]+=cur[i][j];   // 每一步必移动:转移到四邻格
				}
			}
		for(int i=0;i<n;++i)
			for(int j=0;j<m;++j)
				cur[i][j]=nxt[i][j];
	}
	cout<<cur[r2][c2]<<'\n';       // 恰好 T 秒后落在终点的方案数
	return 0;
}

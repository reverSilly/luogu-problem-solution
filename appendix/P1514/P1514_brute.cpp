//
// 题目：P1514 [NOIP 2010 提高组] 引水入城
// 暴力基准: 逐个顶行源点独立 BFS 求精确可达底行位掩码(不依赖区间连续性引理), 再 2^m 子集枚举最少蓄水厂
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{25};
constexpr int dr[4]{-1,1,0,0};
constexpr int dc[4]{0,0,-1,1};

int n,m;
int h[mxn][mxn];
bool vis[mxn][mxn];
int mask[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		for(int j=0;j<m;++j)
			cin>>h[i][j];

	int all=0;
	for(int s=0;s<m;++s)
	{
		fill_n(&vis[0][0],mxn*mxn,false);
		queue<pair<int,int>>Q;
		Q.push({0,s});
		vis[0][s]=true;
		while(!Q.empty())
		{
			auto[r,c]=Q.front();Q.pop();
			for(int d=0;d<4;++d)
			{
				int nr=r+dr[d],nc=c+dc[d];
				if(nr>=0&&nr<n&&nc>=0&&nc<m&&!vis[nr][nc]&&h[nr][nc]<h[r][c])
				{
					vis[nr][nc]=true;
					Q.push({nr,nc});
				}
			}
		}
		int msk=0;
		for(int j=0;j<m;++j)
			if(vis[n-1][j])
				msk|=(1<<j);
		mask[s]=msk;
		all|=msk;
	}

	int full=(1<<m)-1;
	if(all!=full)
	{
		int miss=m-__builtin_popcount(all);
		cout<<0<<'\n'<<miss<<'\n';
		return 0;
	}

	int ans=m;
	for(int sub=0;sub<(1<<m);++sub)
	{
		int cov=0;
		for(int j=0;j<m;++j)
			if(sub>>j&1)
				cov|=mask[j];
		if(cov==full)
			ans=min(ans,__builtin_popcount(sub));
	}
	cout<<1<<'\n'<<ans<<'\n';
	return 0;
}

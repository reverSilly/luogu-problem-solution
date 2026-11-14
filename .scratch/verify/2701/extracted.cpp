// 题目：P2701 [USACO5.3] 巨大的牛棚 Big Barn
// dp[i][j]=以 (i,j) 为右下角的最大空正方形边长
// 空位 min(上,左,左上)+1；树格 0；答案=max dp
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};

int n,t,dp[mxn][mxn],ans;
bool tr[mxn][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>t;
	for(int i=0,x,y;i<t;++i)
	{
		cin>>x>>y;--x;--y;
		tr[x][y]=true;
	}
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
		{
			if(tr[i][j])continue;	// 树格 dp=0，全局默认值正好
			int u=i?dp[i-1][j]:0,v=j?dp[i][j-1]:0,d=(i&&j)?dp[i-1][j-1]:0;
			dp[i][j]=min({u,v,d})+1;
			ans=max(ans,dp[i][j]);
		}
	cout<<ans<<'\n';
	return 0;
}

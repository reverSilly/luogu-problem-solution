//
// 题目：P1641 [SCOI2010] 生成字符串
// 暴力基准：二维格路 DP f[i][j] = 放 i 个 1 和 j 个 0 且全程 i >= j 的方案数
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};
constexpr int p{20100403};

int n,m,dp[mxn][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	if(n<m)
	{
		cout<<0<<'\n';
		return 0;
	}
	dp[0][0]=1;
	for(int i=0;i<=n;++i)
		for(int j=0;j<=min(i,m);++j)
		{
			if(i>0&&(i-1)>=j)
				dp[i][j]=(dp[i][j]+dp[i-1][j])%p;
			if(j>0)
				dp[i][j]=(dp[i][j]+dp[i][j-1])%p;
		}
	cout<<dp[n][m]<<'\n';
	return 0;
}

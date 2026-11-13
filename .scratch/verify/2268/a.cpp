// 题目：P2268 [HNOI2002] DNA 分子的最佳比对
// dp[i][j]：T1 前 i 个字符与 T2 前 j 个字符的最大得分
// 最后一列三种情况：配对 / T1 对空 / T2 对空
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005};

string t1,t2;
int dp[mxn][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>t1>>t2;
	for(char&c:t1)
		c=toupper((unsigned char)c);
	for(char&c:t2)
		c=toupper((unsigned char)c);
	int n=t1.size(),m=t2.size();
	for(int i=1;i<=n;++i)              // 只能一路对空位
		dp[i][0]=dp[i-1][0]-2;
	for(int j=1;j<=m;++j)
		dp[0][j]=dp[0][j-1]-2;
	for(int i=1;i<=n;++i)
		for(int j=1;j<=m;++j)
		{
			int best=max(dp[i-1][j],dp[i][j-1])-2;
			best=max(best,dp[i-1][j-1]+(t1[i-1]==t2[j-1]?1:0));
			dp[i][j]=best;
		}
	cout<<dp[n][m]<<'\n';
	return 0;
}

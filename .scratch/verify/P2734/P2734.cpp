// 题目：P2734 [IOI 1996 / USACO3.3] 游戏 A Game
// 思路：区间 DP。dp[i][j] = 轮到取数的人从区间 [i,j] 能拿到的最大总分（双方最优）。
//       取左端 a[i] 后对手在 [i+1,j] 拿 dp[i+1][j]，我得 sum-dp[i+1][j]；取右端同理。
//       dp[i][j]=sum(i,j)-min(dp[i+1][j],dp[i][j-1])，按区间长度递增递推。
//       先手=dp[0][n-1]，后手=总和-先手。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};

int n,a[mxn],s[mxn],dp[mxn][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
	{
		cin>>a[i];
		s[i+1]=s[i]+a[i];	// 前缀和，sum(i,j)=s[j+1]-s[i]
	}
	for(int i=0;i<n;++i)
		dp[i][i]=a[i];
	for(int len=2;len<=n;++len)
		for(int i=0,j;(j=i+len-1)<n;++i)
			dp[i][j]=s[j+1]-s[i]-min(dp[i+1][j],dp[i][j-1]);
	int first=dp[0][n-1];
	cout<<first<<' '<<s[n]-first<<'\n';
	return 0;
}
// 题目：P2722 [USACO3.1] 总分 Score Inflation
// 完全背包：每类题可选无限次，dp[j]=总耗时不超过 j 的最大得分。
// 正序枚举 j，dp[j-t] 已含本类题 ⇒ 允许重复选（0/1 背包才倒序）。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxm{10005};

int m,n,dp[mxm];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>m>>n;
	for(int i=0,p,t;i<n;++i)
	{
		cin>>p>>t;
		for(int j=t;j<=m;++j)
			dp[j]=max(dp[j],dp[j-t]+p);
	}
	cout<<dp[m]<<'\n';
	return 0;
}
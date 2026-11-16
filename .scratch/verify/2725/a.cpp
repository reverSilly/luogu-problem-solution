// 题目：P2725 [USACO3.1] 邮票 Stamps
// dp[v]=凑出面值 v 所需的最少邮票张数（每种面值无限张，完全背包求最小件数）。
// 答案=第一个 dp[v]>k 的 v 减 1；若 dp[1]>k 则输出 0。
// 上界：k 张邮票最大和为 k*max(a)，故只需算到 k*max(a)+1。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{55},mxv{2000005};	// mxv=k*a_max+1=200*10000+1，+5 松弛
constexpr int inf{1145141919};

int k,n,a[mxn],dp[mxv];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>k>>n;
	int mx=0;
	for(int i=0;i<n;++i)
	{
		cin>>a[i];
		mx=max(mx,a[i]);
	}
	fill_n(dp,k*mx+2,inf);	// 全局默认 0 会被当成「0 张可达」，必须显式填 INF
	dp[0]=0;
	int ans=0;
	for(int v=1;v<=k*mx+1;++v)
	{
		for(int i=0;i<n;++i)
			if(a[i]<=v)
				dp[v]=min(dp[v],dp[v-a[i]]+1);
		if(dp[v]>k)	// 第一个断点：v 不可达，答案 v-1
		{
			ans=v-1;
			break;
		}
	}
	cout<<ans<<'\n';
	return 0;
}

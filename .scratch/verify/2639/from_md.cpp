// 题目：P2639 Bessie's Weight Problem G
// 思路：0/1 背包，价值=重量 ⇒ 求不超过 H 的最大子集和。
//       dp[j]=max(dp[j],dp[j-w]+w)，内层倒序保证每捆只吃一次。
//       dp 全 0 初始化（空集合法），答案 dp[H]。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxh{45005};

int H,n,dp[mxh];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>H>>n;
	for(int i=0,w;i<n;++i)
	{
		cin>>w;
		for(int j=H;j>=w;--j)		// 必须倒序：用到上一层 dp[j-w]
			dp[j]=max(dp[j],dp[j-w]+w);
	}
	cout<<dp[H]<<'\n';
	return 0;
}

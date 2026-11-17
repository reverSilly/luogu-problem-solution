// 题目：P2925 [USACO08DEC] Hay For Sale S
// 价值=重量的 0/1 背包：dp[j]=容量 j 内最大可达子集和。
// 倒序循环 ⇒ dp[j-v] 是上一轮的值 ⇒ 每捆至多买一次。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxc{50005};

int c,h,dp[mxc];	// 最大化+空集合法 ⇒ 全局默认 0 恰好是正确初值，别 fill INF

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>c>>h;
	for(int i=0,v;i<h;++i)
	{
		cin>>v;
		for(int j=c;j>=v;--j)
			dp[j]=max(dp[j],dp[j-v]+v);	// 倒序！正序会重复买同一捆
	}
	cout<<dp[c]<<'\n';
	return 0;
}

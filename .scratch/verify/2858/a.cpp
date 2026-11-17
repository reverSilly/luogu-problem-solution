// 题目：P2858 [USACO06FEB] Treats for the Cows G/S
// 思路：剩余零食恒为区间 [l,r]，今天倍数 day=N-(r-l) 由长度定 ⇒ 区间 DP。
//       dp[l][r]=max(v[l]*day+dp[l+1][r], v[r]*day+dp[l][r-1])。
//       只依赖 len-1 层 ⇒ 一维滚动；l 递增覆盖，f[l+1] 与 f[l] 都还是旧值。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{2005};

int n,v[mxn];
i64 f[mxn];	// 滚动：f[l]=dp[l][l+len-1]；收益上界 2e9 贴 int 上限，用 i64

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>v[i];
	for(int i=0;i<n;++i)
		f[i]=(i64)v[i]*n;	// len=1：只剩它自己，第 n 天卖
	for(int len=2;len<=n;++len)
	{
		int day=n-len+1;
		for(int l=0;l+len<=n;++l)	// l 递增：f[l+1] 尚未被本轮覆盖，仍是 len-1 的值
			f[l]=max(f[l+1]+(i64)v[l]*day,f[l]+(i64)v[l+len-1]*day);
	}
	cout<<f[0]<<'\n';
	return 0;
}

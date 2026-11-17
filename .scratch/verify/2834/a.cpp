// 题目：P2834 纸币问题 3
// 组合(无序)计数 ⇒ 完全背包：面额外层 + 金额升序内层。
// dp[j] = 只用已处理面额凑出 j 的组合数；dp[0]=1(空组合)。
// 升序保证 dp[j-a[i]] 是本轮已更新值(允许再用第 i 种) ⇒ 无限张。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{1005},mxw{10005};
constexpr int p{1'000'000'007};

int n,w,a[mxn],dp[mxw];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>w;
	for(int i=0;i<n;++i)
		cin>>a[i];
	dp[0]=1;					// 凑 0 元：一张都不选，唯一组合
	for(int i=0;i<n;++i)		// 外层枚举纸币种类 ⇒ 组合（无序）
		for(int j=a[i];j<=w;++j)
		{
			dp[j]+=dp[j-a[i]];	// 两个 <p 的数相加 <2p<2^31，int 安全
			if(dp[j]>=p)
				dp[j]-=p;
		}
	cout<<dp[w]<<'\n';
	return 0;
}

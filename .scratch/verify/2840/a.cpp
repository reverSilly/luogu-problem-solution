// 题目：P2840 纸币问题 2
// 排列(有序)计数 ⇒ 金额外层 + 面额内层：dp[j]=Σ dp[j-a[i]]。
// 按"最后一张是 a[i]"分类；dp[0]=1(空序列)。
// 与 P2834(组合,面额外层)恰为孪生，循环次序相反。
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
	dp[0]=1;					// 支付 0 元：空序列，唯一方式
	for(int j=1;j<=w;++j)		// 外层枚举金额 ⇒ 排列（有序）
		for(int i=0;i<n;++i)
			if(a[i]<=j)
			{
				dp[j]+=dp[j-a[i]];	// 最后一张是 a[i]
				if(dp[j]>=p)
					dp[j]-=p;
			}
	cout<<dp[w]<<'\n';
	return 0;
}

//
// 题目：P1616 疯狂的采药
// https://www.luogu.com.cn/problem/P1616
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxm{10005};
constexpr int mxt{10000005};

int t,m,a[mxm],b[mxm];
i64 dp[mxt];                       // dp[j]：容量不超过 j 时的最大价值

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>t>>m;
	for(int i=0;i<m;++i)
		cin>>a[i]>>b[i];
	for(int i=0;i<m;++i)
		for(int j=a[i];j<=t;++j)   // 正序：同一物品可重复选取
			dp[j]=max(dp[j],dp[j-a[i]]+b[i]);
	cout<<dp[t]<<'\n';
	return 0;
}

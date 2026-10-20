//
// 题目：P1569 [USACO11FEB] Generic Cow Protests
// https://www.luogu.com.cn/problem/P1569
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{1005};          // n<=1000 (+5 松弛)

int n;
i64 pre[mxn];                     // pre[i]=前 i 头牛的理智度前缀和(0-based 端点)
int dp[mxn];                      // dp[i]=把前 i 头牛合法分组的最大组数

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
	{
		i64 x;
		cin>>x;
		pre[i+1]=pre[i]+x;
	}

	fill_n(&dp[0],mxn,-1);        // -1 表示该前缀不可达;全局数组默认 0 会误判可达
	dp[0]=0;                      // 空前缀:0 组
	for(int i=1;i<=n;++i)
		for(int j=0;j<i;++j)
			if(dp[j]>=0&&pre[j]<=pre[i])   // 区间(j,i]的和 pre[i]-pre[j]>=0
				dp[i]=max(dp[i],dp[j]+1);

	if(dp[n]<0)
		cout<<"Impossible\n";     // 无合法分组 <=> 总和为负
	else
		cout<<dp[n]<<'\n';
	return 0;
}

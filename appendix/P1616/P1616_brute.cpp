//
// P1616 朴素解：二维完全背包（按物品种类逐层转移）
// 用法：仅用于与 P1616.cpp 对拍，非提交代码
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxm{105};
constexpr int mxt{2005};

int m,t,a[mxm],b[mxm];
i64 dp[mxm][mxt];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>t>>m;
	for(int i=1;i<=m;++i)
		cin>>a[i]>>b[i];
	for(int i=1;i<=m;++i)
		for(int j=0;j<=t;++j)
		{
			dp[i][j]=dp[i-1][j];
			if(j>=a[i])
				dp[i][j]=max(dp[i][j],dp[i][j-a[i]]+b[i]);
		}
	cout<<dp[m][t]<<'\n';
	return 0;
}

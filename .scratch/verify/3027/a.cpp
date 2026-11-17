// 题目：P3027 [USACO10OCT] Making Money G
// 完全背包：体积 C_i，价值 R_i-C_i，容量 M；利润按样例口径 = sum((R-C)*x)
// 全 0 初始化 = "预算至多 j"语义（空方案永远可行）；j 正序 = 同种无限件
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105},mxm{100005};

int n,m,c[mxn],v[mxn],dp[mxm];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0,r;i<n;++i)
	{
		cin>>c[i]>>r;
		v[i]=r-c[i];
	}
	for(int i=0;i<n;++i)
	{
		if(v[i]<=0)
			continue;	// 非正利润：删掉不劣（引理 2）
		for(int j=c[i];j<=m;++j)
			dp[j]=max(dp[j],dp[j-c[i]]+v[i]);	// 正序：dp[j-c] 可能已含第 i 种
	}
	cout<<dp[m]<<'\n';
	return 0;
}

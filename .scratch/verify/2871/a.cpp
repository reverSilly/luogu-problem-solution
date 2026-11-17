// 题目：P2871 [USACO07DEC] Charm Bracelet S
// 思路：0/1 背包滚动数组。倒序刷 j，保证 dp[j-w] 还是上一层（不含当前物品）。
//       口径是"总重 <= j"，空集合法 ⇒ 全 0 初始化恰好正确，无需 INF。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxm{12885};

int n,m,dp[mxm];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0,w,d;i<n;++i)
	{
		cin>>w>>d;
		for(int j=m;j>=w;--j)	// 倒序！升序会把当前物品选两次（变完全背包）
			dp[j]=max(dp[j],dp[j-w]+d);
	}
	cout<<dp[m]<<'\n';
	return 0;
}

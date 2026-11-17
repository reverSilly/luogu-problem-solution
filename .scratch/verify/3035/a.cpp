// 题目：P3035 [USACO11DEC] Umbrellas for Cows S
// 后缀最小化 c[w]=min_{w'>=w}C[w'] ⇒ 存在"连续分段"最优解 ⇒ dp[i]=min dp[j]+c[跨度]
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{5005},mxm{100005};
constexpr i64 inf{1145141919810};

int n,m,x[mxn],c[mxm];
i64 dp[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<n;++i)
		cin>>x[i];
	for(int w=1;w<=m;++w)
		cin>>c[w];
	sort(x,x+n);
	for(int w=m-1;w>=1;--w)
		c[w]=min(c[w],c[w+1]);	// 大伞能盖小范围，有效代价单调不增
	fill_n(dp,n+1,inf);
	dp[0]=0;
	for(int i=1;i<=n;++i)
		for(int j=0;j<i;++j)
			dp[i]=min(dp[i],dp[j]+c[x[i-1]-x[j]+1]);	// 最后一段伞盖第j..i-1头牛
	cout<<dp[n]<<'\n';
	return 0;
}

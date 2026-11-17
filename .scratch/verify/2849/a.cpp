// 题目：P2849 [USACO14DEC] Marathon S
// dp[i][j]=到点 i（保留）、1..i-1 中恰跳 j 个的最短曼哈顿路程
// 枚举上一保留点 p：p+1..i-1 全跳（i-p-1 个），dp[i][j]=min(dp[p][j-(i-p-1)]+d(p,i))
// 三角不等式 ⇒ 多跳不亏 ⇒ "恰好 K"="至多 K"，答案 dp[n-1][min(k,n-2)]
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{505};
constexpr int inf{1145141919};

int n,k,x[mxn],y[mxn],dp[mxn][mxn];

int dis(int u,int v)
{
	return abs(x[u]-x[v])+abs(y[u]-y[v]);
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k;
	for(int i=0;i<n;++i)
		cin>>x[i]>>y[i];
	fill_n(&dp[0][0],mxn*mxn,inf);	// 全局默认 0 是合法路程，必须显式填 INF
	dp[0][0]=0;
	for(int i=1;i<n;++i)
		for(int j=0;j<=k&&j<=i-1;++j)
			for(int p=i-1;p>=0&&i-p-1<=j;--p)	// p+1..i-1 全跳，共 i-p-1 个
				dp[i][j]=min(dp[i][j],dp[p][j-(i-p-1)]+dis(p,i));
	cout<<dp[n-1][min(k,n-2)]<<'\n';
	return 0;
}

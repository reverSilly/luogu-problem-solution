// 题目：P2733 [USACO3.3] 家的范围 Home on the Range
// 思路：dp[i][j] = 以 (i,j) 为右下角的最大全 1 正方形边长。
//       g=1 时 dp[i][j]=min(上,左,左上)+1，否则 0；第 0 行/列作哨兵 0（全局默认）。
//       边长 s 的正方形个数 = dp>=s 的格子数 = freq 的后缀和。输出 s>=2 且个数>0 的行。
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{255};

int n,dp[mxn][mxn],freq[mxn];
char g[mxn][mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>g[i];
	for(int i=1;i<=n;++i)
		for(int j=1;j<=n;++j)
			if(g[i-1][j-1]=='1')
			{
				dp[i][j]=min({dp[i-1][j],dp[i][j-1],dp[i-1][j-1]})+1;
				++freq[dp[i][j]];
			}
	for(int s=n-1;s>=1;--s)	// 后缀和：freq[s] 变为「dp>=s 的格子数」
		freq[s]+=freq[s+1];
	for(int s=2;s<=n;++s)
		if(freq[s])
			cout<<s<<' '<<freq[s]<<'\n';
	return 0;
}

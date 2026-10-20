//
// 题目：P1559 运动员最佳匹配问题
// https://www.luogu.com.cn/problem/P1559
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{25};                                // n<=20 (+5 松弛)
constexpr int mxs{1<<20};
constexpr i64 nf{-1145141919810LL};                   // 114514 家族哨兵

int n;
int P[mxn][mxn],Q[mxn][mxn];
i64 dp[mxs];                                          // dp[mask]=已把前 popcount(mask) 名男生配给 mask 中的女生时的最大优势和

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			cin>>P[i][j];
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			cin>>Q[i][j];

	int lim=1<<n;
	fill_n(&dp[0],mxs,nf);                            // 全局数组默认 0,必须显式填哨兵
	dp[0]=0;
	for(int mask=0;mask<lim;++mask)
	{
		if(dp[mask]==nf)
			continue;
		int k=__builtin_popcount((unsigned)mask);     // 已配好的男生数
		if(k==n)
			continue;
		for(int j=0;j<n;++j)
			if(!(mask>>j&1))
			{
				int nm=mask|(1<<j);
				i64 v=dp[mask]+(i64)P[k][j]*Q[j][k];  // 第 k 名男生 配 第 j 名女生
				if(v>dp[nm])
					dp[nm]=v;
			}
	}
	cout<<dp[lim-1]<<'\n';
	return 0;
}

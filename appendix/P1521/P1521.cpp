//
// 题目：P1521 求逆序对
// https://www.luogu.com.cn/problem/P1521
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mod{10000};         // 题面要求对 10000 取模
constexpr int mxn{105};           // N<=100 (+5 松弛)
constexpr int mxk{5005};          // K <= 100*99/2 = 4950 (+5 松弛)

int n,k;
int dp[mxn][mxk];                 // dp[i][j]=1..i 的排列中逆序对恰为 j 的方案数

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cin>>n>>k;
	dp[0][0]=1;                  // 空排列逆序对为 0,方案数 1
	for(int i=1;i<=n;++i)
		for(int j=0;j<=k;++j)
		{
			i64 v=dp[i-1][j];        // 未在末尾制造新逆序对
			if(j>0)
				v+=dp[i][j-1];       // 前缀和滑动:dp[i][j-1]
			if(j>=i)
				v-=dp[i-1][j-i];     // 滑出窗口的旧项
			v%=mod;
			if(v<0)
				v+=mod;
			dp[i][j]=int(v);
		}
	cout<<dp[n][k]<<'\n';
	return 0;
}

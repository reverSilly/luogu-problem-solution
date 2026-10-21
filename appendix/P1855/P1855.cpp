//
// 题目：P1855 榨取kkksc03
// https://www.luogu.com.cn/problem/P1855
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxm{205};
constexpr int mxn{105};

int n,mny,tim;
int dp[mxm][mxm];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>mny>>tim;
	for(int i=0;i<n;++i)
	{
		int c,t;
		cin>>c>>t;
		if(c>mny||t>tim)
			continue;
		for(int j=mny;j>=c;--j)
			for(int k=tim;k>=t;--k)
				dp[j][k]=max(dp[j][k],dp[j-c][k-t]+1);
	}
	cout<<dp[mny][tim]<<'\n';
	return 0;
}

//
// 题目：P1622 释放囚犯
// https://www.luogu.com.cn/problem/P1622
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxq{105};
constexpr int inf{1145141919};

int P,Q;
int a[mxq];                        // a[0]=0, a[1..Q]=名单, a[Q+1]=P+1
int dp[mxq][mxq];                  // dp[i][j]：只释放 (i,j) 之间的囚犯的最小代价

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>P>>Q;
	a[0]=0;
	a[Q+1]=P+1;
	for(int i=1;i<=Q;++i)
		cin>>a[i];
	for(int i=0;i<=Q;++i)
		for(int j=0;j<=Q+1;++j)
			dp[i][j]=(j>i+1?inf:0);
	for(int len=2;len<=Q+1;++len)
		for(int i=0;i+len<=Q+1;++i)
		{
			int j=i+len;
			// 区间内还有 a[j]-a[i]-1 人，被放走的那人不喊，故为人数减一
			int w=a[j]-a[i]-2;
			for(int k=i+1;k<j;++k)
				dp[i][j]=min(dp[i][j],dp[i][k]+dp[k][j]+w);
		}
	cout<<dp[0][Q+1]<<'\n';
	return 0;
}

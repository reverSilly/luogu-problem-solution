//
// 题目：P1564 膜拜
// https://www.luogu.com.cn/problem/P1564
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{2505};          // n<=2500 (+5 松弛)
constexpr int inf{1145141919};

int n,m;
int pre[mxn];                     // pre[i]=前 i 个同学中崇拜甲(1)的人数
int dp[mxn];                      // dp[i]=前 i 个同学最少需要的机房数

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=1,x;i<=n;++i)
	{
		cin>>x;
		pre[i]=pre[i-1]+(x==1);
	}
	fill_n(&dp[0],mxn,inf);
	dp[0]=0;
	for(int i=1;i<=n;++i)
		for(int j=0;j<i;++j)
		{
			int c1=pre[i]-pre[j];
			int len=i-j;
			int c2=len-c1;
			if(c1==0||c2==0||abs(c1-c2)<=m)   // 全同 或 两派人数差<=m
				dp[i]=min(dp[i],dp[j]+1);
		}
	cout<<dp[n]<<'\n';
	return 0;
}

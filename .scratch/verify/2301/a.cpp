// 题目：P2301 就是干！
// 思路：排序后，最优方案里每对木棍一定是排序数组中相邻的两根
//       dp[i][j] = 前 i 根木棍里配出 j 对的最小不舒服指数
//       转移：丢掉第 i 根 / 第 i 根与第 i-1 根配成一对
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{2005},mxk{505};
constexpr i64 inf{1145141919810};

int m,n;
i64 a[mxn];
i64 dp[mxn][mxk];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>m>>n;
	for(int i=1;i<=m;++i)
		cin>>a[i];
	sort(a+1,a+m+1);

	fill_n(&dp[0][0],mxn*mxk,inf);
	dp[0][0]=0;
	for(int i=1;i<=m;++i)
		for(int j=0;j<=n;++j)
		{
			dp[i][j]=dp[i-1][j];
			if(j>0&&i>=2&&dp[i-2][j-1]<inf)
				dp[i][j]=min(dp[i][j],dp[i-2][j-1]+(a[i]-a[i-1])*(a[i]-a[i-1]));
		}
	cout<<dp[m][n]<<'\n';
	return 0;
}

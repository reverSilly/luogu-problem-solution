//
// 题目：P1650 田忌赛马
// 暴力基准：O(n^2) 区间/首尾 DP 求最大净胜银币
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{505};
constexpr int inf{1145141919};

int n,a[mxn],b[mxn],dp[mxn][mxn];

int score(int x,int y)
{
	if(x>y)
		return 200;
	if(x<y)
		return -200;
	return 0;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i)
		cin>>a[i];
	for(int i=0;i<n;++i)
		cin>>b[i];

	sort(a,a+n,greater<>());
	sort(b,b+n,greater<>());

	for(int i=0;i<=n;++i)
		for(int j=0;j<=n;++j)
			dp[i][j]=-inf;
	dp[0][0]=0;

	for(int i=1;i<=n;++i)
		for(int j=0;j<=i;++j)
		{
			if(j>0&&dp[i-1][j-1]!=-inf)
				dp[i][j]=max(dp[i][j],dp[i-1][j-1]+score(a[j-1],b[i-1]));
			if(j<i&&dp[i-1][j]!=-inf)
				dp[i][j]=max(dp[i][j],dp[i-1][j]+score(a[n-(i-j)],b[i-1]));
		}

	int ans=-inf;
	for(int j=0;j<=n;++j)
		ans=max(ans,dp[n][j]);
	cout<<ans<<'\n';
	return 0;
}

//
// 题目：P1541 [NOIP 2010 提高组] 乌龟棋
// https://www.luogu.com.cn/problem/P1541
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{355};           // N<=350 (+5 松弛)
constexpr int mxc{45};            // 每种卡片不超过 40 张 (+5 松弛)

int n,m;
int a[mxn],cnt[5];
int dp[mxc][mxc][mxc][mxc];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n>>m))
		return 0;
	for(int i=0;i<n;++i)
		cin>>a[i];
	for(int i=0;i<m;++i)
	{
		int x;
		cin>>x;
		++cnt[x];
	}

	for(int i=0;i<=cnt[1];++i)
		for(int j=0;j<=cnt[2];++j)
			for(int k=0;k<=cnt[3];++k)
				for(int l=0;l<=cnt[4];++l)
				{
					int best=0;
					if(i>0)
						best=max(best,dp[i-1][j][k][l]);
					if(j>0)
						best=max(best,dp[i][j-1][k][l]);
					if(k>0)
						best=max(best,dp[i][j][k-1][l]);
					if(l>0)
						best=max(best,dp[i][j][k][l-1]);
					int pos=i+2*j+3*k+4*l;
					dp[i][j][k][l]=best+a[pos];
				}

	cout<<dp[cnt[1]][cnt[2]][cnt[3]][cnt[4]]<<'\n';
	return 0;
}

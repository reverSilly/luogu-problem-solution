//
// 题目：P1508 Likecloud-吃、吃、吃
// https://www.luogu.com.cn/problem/P1508
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{205};        // 行数 m<=200 (+5 松弛)
constexpr int mxm{205};        // 列数 n<=200 (+5 松弛)
constexpr int inf{1145141919}; // 114514 家族哨兵, 取负当 -inf

int m,n;
int a[mxn][mxm];
int dp[mxn][mxm];              // dp[i][j]=从底部中点走到 (i,j) 的最大能量和

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>m>>n;
	for(int i=0;i<m;++i)
		for(int j=0;j<n;++j)
			cin>>a[i][j];

	for(int i=0;i<=m;++i)
		for(int j=0;j<n;++j)
			dp[i][j]=-inf;
	int mid=n>>1;              // n 为奇数, 0-based 的中间列
	dp[m][mid]=0;              // 虚拟第 m 行只在中点列可达, 能量 0

	for(int i=m-1;i>=0;--i)
		for(int j=0;j<n;++j)
		{
			int bst=-inf;
			for(int k=-1;k<=1;++k)
			{
				int c=j+k;
				if(c<0||c>=n)
					continue;
				bst=max(bst,dp[i+1][c]);
			}
			if(bst!=-inf)
				dp[i][j]=a[i][j]+bst;
		}

	int ans=-inf;
	for(int j=0;j<n;++j)
		ans=max(ans,dp[0][j]);
	cout<<ans<<'\n';
	return 0;
}

// 题目：P2364 胖男孩
// 三串最长公共子序列 + 回溯还原
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};

int dp[mxn][mxn][mxn];
string a,b,c;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>a>>b>>c;
	int n=a.size(),m=b.size(),l=c.size();
	for(int i=1;i<=n;++i)
		for(int j=1;j<=m;++j)
			for(int k=1;k<=l;++k)
				if(a[i-1]==b[j-1]&&a[i-1]==c[k-1])
					dp[i][j][k]=dp[i-1][j-1][k-1]+1;   // 三者相等一定取
				else
					dp[i][j][k]=max({dp[i-1][j][k],dp[i][j-1][k],dp[i][j][k-1]});
	string ans;
	int i=n,j=m,k=l;
	while(i&&j&&k)
	{
		if(a[i-1]==b[j-1]&&a[i-1]==c[k-1])
		{
			ans+=a[i-1];
			--i;--j;--k;
		}
		else if(dp[i][j][k]==dp[i-1][j][k])
			--i;
		else if(dp[i][j][k]==dp[i][j-1][k])
			--j;
		else
			--k;
	}
	reverse(ans.begin(),ans.end());
	cout<<ans<<'\n';
	return 0;
}

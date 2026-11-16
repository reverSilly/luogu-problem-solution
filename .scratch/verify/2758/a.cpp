// 题目：P2758 编辑距离
// dp[i][j]：A 的前 i 个字符变成 B 的前 j 个字符的最少操作数
// dp[i][j]=min(dp[i-1][j]+1, dp[i][j-1]+1, dp[i-1][j-1]+(a[i-1]!=b[j-1]))
//          删 A 末尾     插入 B 末尾     替换/匹配末尾
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{2005};

int n,m,dp[mxn][mxn];
string a,b;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>a>>b;
	n=a.size();m=b.size();
	for(int i=0;i<=n;++i)
		dp[i][0]=i;	// 全删
	for(int j=0;j<=m;++j)
		dp[0][j]=j;	// 全插
	for(int i=1;i<=n;++i)
		for(int j=1;j<=m;++j)
			dp[i][j]=min({dp[i-1][j]+1,dp[i][j-1]+1,dp[i-1][j-1]+(a[i-1]!=b[j-1])});
	cout<<dp[n][m]<<'\n';
	return 0;
}

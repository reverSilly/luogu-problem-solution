#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};

int dp[mxn][mxn];
string a,b;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>a>>b;
	int la=a.size(),lb=b.size();
	for(int i=1;i<=la;++i)
		for(int j=1;j<=lb;++j)
			if(a[i-1]==b[j-1])
				dp[i][j]=dp[i-1][j-1]+1;
			else
				dp[i][j]=max(dp[i-1][j],dp[i][j-1]);

	int L=dp[la][lb];//最长公共子序列长度
	int n=max(la,lb),m=min(la,lb);
	int ans=L,cnt=0;
	while(ans*2<n)
	{
		++cnt;
		ans+=m;
		m*=2;
	}
	cout<<cnt+1<<'\n';
	return 0;
}

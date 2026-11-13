#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{10005};

int n,m,t[mxn],x[mxn],y[mxn],dp[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	for(int i=0;i<m;++i)
		cin>>t[i]>>x[i]>>y[i];
	int ans=0;
	for(int i=0;i<m;++i)
	{
		dp[i]=1;
		for(int j=0;j<i;++j)
			if(t[i]-t[j]>=abs(x[i]-x[j])+abs(y[i]-y[j]))
				dp[i]=max(dp[i],dp[j]+1);
		ans=max(ans,dp[i]);
	}
	cout<<ans<<'\n';
	return 0;
}

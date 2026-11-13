#include<bits/stdc++.h>
using namespace std;

constexpr int mxv{400000},mxn{800005};
constexpr int neg{-1145141919};

int n;
int dp[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	fill_n(dp,mxn,neg);
	dp[mxv]=0;
	int lo=0,hi=0;
	cin>>n;
	for(int i=0,s,f;i<n;++i)
	{
		cin>>s>>f;
		if(s>=0)
		{
			for(int j=hi+s;j>=lo+s;--j)
				if(dp[j-s+mxv]!=neg)
					dp[j+mxv]=max(dp[j+mxv],dp[j-s+mxv]+f);
			hi+=s;
		}
		else
		{
			for(int j=lo+s;j<=hi+s;++j)
				if(dp[j-s+mxv]!=neg)
					dp[j+mxv]=max(dp[j+mxv],dp[j-s+mxv]+f);
			lo+=s;
		}
	}
	int ans=0;
	for(int j=0;j<=hi;++j)
		if(dp[j+mxv]>=0)
			ans=max(ans,j+dp[j+mxv]);
	cout<<ans<<'\n';
	return 0;
}

#include<bits/stdc++.h>
using namespace std;

int n;
int p[22][22],q[22][22],w[22][22];
int dp[1<<20];

int main()
{
	cin>>n;
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			cin>>p[i][j];
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			cin>>q[i][j];
	for(int i=0;i<n;++i)
		for(int j=0;j<n;++j)
			w[i][j]=p[i][j]*q[j][i];
	int lim=1<<n;
	fill(dp,dp+lim,-1e9);
	dp[0]=0;
	for(int mask=0;mask<lim-1;++mask)
	{
		int i=__builtin_popcount(mask);
		for(int j=0;j<n;++j)
			if(!((mask>>j)&1))
				dp[mask|(1<<j)]=max(dp[mask|(1<<j)],dp[mask]+w[i][j]);
	}
	cout<<dp[lim-1]<<'\n';
	return 0;
}

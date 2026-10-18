#include<bits/stdc++.h>
using namespace std;

int main()
{
	int n,L,A,B;
	cin>>n>>L>>A>>B;
	vector<pair<int,int>> cows(n);
	for(int i=0;i<n;++i)
		cin>>cows[i].first>>cows[i].second;
	vector<bool> bad(L+1,false);
	for(auto [s,e]:cows)
		for(int x=s+1;x<e;++x)
			bad[x]=true;
	const int inf=1e9;
	vector<int> dp(L+1,inf);
	dp[0]=0;
	for(int x=2;x<=L;x+=2)
	{
		if(bad[x])
			continue;
		for(int r=A;r<=B;++r)
			if(x-2*r>=0&&dp[x-2*r]<inf)
				dp[x]=min(dp[x],dp[x-2*r]+1);
	}
	cout<<(dp[L]>=inf?-1:dp[L])<<'\n';
	return 0;
}

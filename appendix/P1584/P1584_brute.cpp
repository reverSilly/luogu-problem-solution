#include<bits/stdc++.h>
using namespace std;

int main()
{
	int n;
	long long low,high;
	cin>>n>>low>>high;
	vector<long long> pl(n+1,0),pm(n+1,0);
	for(int i=1;i<=n;++i)
	{
		long long x;
		cin>>x;
		pl[i]=pl[i-1]+x;
	}
	for(int i=1;i<=n;++i)
	{
		long long x;
		cin>>x;
		pm[i]=pm[i-1]+x;
	}
	struct Seg
	{
		int l,r;
		long long w;
	};
	vector<Seg> segs;
	for(int i=1;i<=n;++i)
		for(int j=i;j<=n;++j)
		{
			long long len=pl[j]-pl[i-1];
			if(len>=low&&len<=high)
				segs.push_back({i,j,pm[j]-pm[i-1]});
		}
	int k=segs.size();
	vector<long long> dp(k,0);
	long long ans=0;
	for(int i=0;i<k;++i)
	{
		dp[i]=segs[i].w;
		for(int j=0;j<i;++j)
			if(segs[j].l<segs[i].l&&segs[j].r<segs[i].r)
				dp[i]=max(dp[i],dp[j]+segs[i].w);
		ans=max(ans,dp[i]);
	}
	cout<<ans<<'\n';
	return 0;
}

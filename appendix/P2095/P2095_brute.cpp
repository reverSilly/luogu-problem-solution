#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{205};

int n,m,k;
int lim[mxn];
int a[mxn],b[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m>>k;
	for(int i=1;i<=k;++i)
		cin>>lim[i];
	for(int i=0;i<n;++i)
		cin>>a[i]>>b[i];

	// 暴力：枚举全部子集，检查总份数与每类上限
	int ans=0;
	for(int mask=0;mask<(1<<n);++mask)
	{
		int cnt[mxn]={0},tot=0,sum=0,num=0;
		for(int i=0;i<n;++i)
			if(mask>>i&1)
			{
				++cnt[b[i]];
				sum+=a[i];
				++num;
			}
		if(num>m)
			continue;
		bool ok=true;
		for(int i=1;i<=k;++i)
			if(cnt[i]>lim[i])
				ok=false;
		if(ok)
			ans=max(ans,sum);
	}
	cout<<ans<<'\n';
	return 0;
}

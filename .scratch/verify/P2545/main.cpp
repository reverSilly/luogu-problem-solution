#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{2005};
constexpr i64 ninf{-(1LL<<60)};

int n;
i64 a[mxn],c[mxn];	// a=第一行前缀和, c=两行合计前缀和

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=1;i<=n;++i)
	{
		i64 x;cin>>x;
		a[i]=a[i-1]+x;
	}
	for(int i=1;i<=n;++i)
	{
		i64 x;cin>>x;
		c[i]=c[i-1]+(a[i]-a[i-1])+x;
	}
	// 凹字型 = 选 x<y<z<w，收益 c[w]-c[x]+a[y]-a[z]
	// dp1=-c[x], dp2=a[y]-c[x], dp3=a[y]-c[x]-a[z]，依次喂给 ans=dp3+c[w]
	i64 dp1=ninf,dp2=ninf,dp3=ninf,ans=ninf;
	for(int i=0;i<=n;++i)
	{
		if(dp3>ninf/2)
			ans=max(ans,dp3+c[i]);
		if(dp2>ninf/2)
			dp3=max(dp3,dp2-a[i]);
		if(dp1>ninf/2)
			dp2=max(dp2,dp1+a[i]);
		dp1=max(dp1,-c[i]);
	}
	cout<<ans<<'\n';
	return 0;
}
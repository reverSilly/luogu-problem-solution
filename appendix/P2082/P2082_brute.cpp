#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005};
constexpr int mxv{100005};

int n;
i64 s[mxn],t[mxn];
bool cov[mxv];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	i64 mx=0;
	for(int i=0;i<n;++i)
	{
		cin>>s[i]>>t[i];
		mx=max(mx,t[i]);
	}
	// 暴力：把每个整数点标出来，统计被覆盖的点数
	for(int i=0;i<n;++i)
		for(i64 x=s[i];x<=t[i];++x)
			cov[x]=true;
	int ans=0;
	for(i64 x=1;x<=mx;++x)
		ans+=cov[x];
	cout<<ans<<'\n';
	return 0;
}

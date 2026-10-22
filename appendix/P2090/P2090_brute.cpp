#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr i64 inf{1LL<<60};

int n;

i64 cnt(i64 a,i64 b)
{
	// 暴力：每次把较大的数减去较小的数，统计回到 (1,1) 的步数
	i64 s=0;
	while(a!=b)
	{
		if(a>b)
			a-=b;
		else
			b-=a;
		++s;
	}
	if(a!=1)
		return inf;
	return s;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	i64 ans=inf;
	for(int i=1;i<=n;++i)
		ans=min(ans,cnt(n,i));
	cout<<ans<<'\n';
	return 0;
}

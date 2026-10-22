// 题目：P2036 PERKET —— 暴力：二进制枚举子集
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{15};

int n;
i64 s[mxn],b[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=0;i<n;++i) cin>>s[i]>>b[i];
	i64 ans=1LL<<62;
	for(int mask=1;mask<(1<<n);++mask)
	{
		i64 prod=1,sum=0;
		for(int i=0;i<n;++i)
			if(mask>>i&1) prod*=s[i],sum+=b[i];
		ans=min(ans,llabs(prod-sum));
	}
	cout<<ans<<'\n';
	return 0;
}

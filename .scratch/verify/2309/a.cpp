// 题目：P2309 loidc，卖卖萌
// 思路：子段 (l..r) 的和 = s[r]-s[l-1]，>0 等价于 s[l-1]<s[r]
//       于是答案 = 下标对 (i<j) 且 s[i]<s[j] 的个数
//       从左往右扫描，树状数组维护已出现前缀和的排名次数
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{100005};

int n;
int a[mxn];
i64 s[mxn],b[mxn],bit[mxn];

void add(int i,int v)
{
	for(;i<=n+1;i+=i&-i)
		bit[i]+=v;
}
i64 sigma(int i)
{
	i64 r=0;
	for(;i>0;i-=i&-i)
		r+=bit[i];
	return r;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	for(int i=1;i<=n;++i)
		cin>>a[i];
	for(int i=1;i<=n;++i)
		s[i]=s[i-1]+a[i];

	for(int i=0;i<=n;++i)
		b[i]=s[i];
	sort(b,b+n+1);
	int k=unique(b,b+n+1)-b;

	i64 ans=0;
	for(int i=0;i<=n;++i)
	{
		int rk=lower_bound(b,b+k,s[i])-b+1;   // 1-based 排名
		ans+=sigma(rk-1);                     // 只数严格小的，差为 0 的不算
		add(rk,1);
	}
	cout<<ans<<'\n';
	return 0;
}

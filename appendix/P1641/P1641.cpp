//
// 题目：P1641 [SCOI2010] 生成字符串
// https://www.luogu.com.cn/problem/P1641
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{2000005};
constexpr i64 p{20100403};

int n,m;
i64 fac[mxn];

i64 qpow(i64 a,i64 b)
{
	i64 r=1;
	for(;b;b>>=1,a=a*a%p)
		if(b&1)
			r=r*a%p;
	return r;
}

i64 C(int a,int b)
{
	if(b<0||b>a)
		return 0;
	return fac[a]*qpow(fac[b]*fac[a-b]%p,p-2)%p;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>m;
	if(n<m)
	{
		cout<<0<<'\n';
		return 0;
	}
	fac[0]=1;
	for(int i=1;i<=n+m;++i)
		fac[i]=fac[i-1]*i%p;

	i64 ans=(C(n+m,m)-C(n+m,m-1)+p)%p;
	cout<<ans<<'\n';
	return 0;
}

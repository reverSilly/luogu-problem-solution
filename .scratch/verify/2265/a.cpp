// 题目：P2265 路边的水沟
// 从右下格点到左上格点，只许向左/向上
// 需要 n-1 次向上、m-1 次向左，方案数 C(n+m-2,n-1)
// 预处理阶乘，费马小定理求逆元
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mxn{2000005};
constexpr i64 p{1'000'000'007};

i64 fac[mxn];

i64 qpow(i64 a,i64 b)
{
	i64 r=1;
	for(;b;b>>=1,a=a*a%p)
		if(b&1)
			r=r*a%p;
	return r;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	int n,m;
	cin>>n>>m;
	int N=n+m-2;
	fac[0]=1;
	for(int i=1;i<=N;++i)
		fac[i]=fac[i-1]*i%p;
	i64 ans=fac[N]*qpow(fac[n-1],p-2)%p*qpow(fac[m-1],p-2)%p;
	cout<<ans<<'\n';
	return 0;
}

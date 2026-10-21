#include<bits/stdc++.h>
using namespace std;

constexpr long long mod=9901;

long long qpow(long long a,long long b)
{
	a%=mod;
	long long r=1;
	while(b)
	{
		if(b&1) r=r*a%mod;
		a=a*a%mod;
		b>>=1;
	}
	return r;
}

// 分治法求 1 + p + ... + p^k (不依赖乘法逆元)
long long sum_div_conq(long long p,long long k)
{
	if(k==0)
		return 1;
	if(k&1)
	{
		long long half=sum_div_conq(p,k>>1);
		return half*(1+qpow(p,(k>>1)+1))%mod;
	}
	else
	{
		long long half=sum_div_conq(p,(k>>1)-1);
		return (half*(1+qpow(p,k>>1))+qpow(p,k))%mod;
	}
}

int main()
{
	long long a,b;
	cin>>a>>b;
	if(a==0){cout<<0<<'\n';return 0;}
	if(b==0){cout<<1<<'\n';return 0;}
	long long ans=1;
	for(long long i=2;i*i<=a;++i)
		if(a%i==0)
		{
			long long c=0;
			while(a%i==0){++c;a/=i;}
			ans=ans*sum_div_conq(i,c*b)%mod;
		}
	if(a>1)
		ans=ans*sum_div_conq(a,b)%mod;
	cout<<ans<<'\n';
	return 0;
}

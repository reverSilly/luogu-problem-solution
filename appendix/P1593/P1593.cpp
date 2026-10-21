//
// 题目：P1593 因子和
// https://www.luogu.com.cn/problem/P1593
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr long long mod{9901};

long long qpow(long long a,long long b)
{
	a%=mod;
	long long res=1;
	while(b>0)
	{
		if(b&1)
			res=res*a%mod;
		a=a*a%mod;
		b>>=1;
	}
	return res;
}

// 计算等比数列 1 + p + p^2 + ... + p^k (mod 9901)
long long geom_sum(long long p,long long k)
{
	p%=mod;
	if(p==0)
		return 1;
	if(p==1)
		return (k+1)%mod;
	long long num=(qpow(p,k+1)-1+mod)%mod;
	long long inv=qpow(p-1,mod-2);
	return num*inv%mod;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	long long a,b;
	if(!(cin>>a>>b))
		return 0;
	if(a==0)
	{
		cout<<0<<'\n';
		return 0;
	}
	if(b==0)
	{
		cout<<1<<'\n';
		return 0;
	}

	long long ans=1;
	for(long long i=2;i*i<=a;++i)
		if(a%i==0)
		{
			long long cnt=0;
			while(a%i==0)
			{
				++cnt;
				a/=i;
			}
			ans=ans*geom_sum(i,cnt*b)%mod;
		}
	if(a>1)
		ans=ans*geom_sum(a,b)%mod;

	cout<<ans<<'\n';
	return 0;
}

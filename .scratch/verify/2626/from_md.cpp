// 题目：P2626 斐波那契数列（升级版）
// 边递推边 mod 2^31，再对结果试除分解质因数
// 输出格式：值=质数^指数*质数^指数...（指数 1 省略；值为 1 时输出 1=1）
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr i64 mod{1LL<<31};

int n;
i64 f[55];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n;
	f[1]=f[2]=1;
	for(int i=3;i<=n;++i)
		f[i]=(f[i-1]+f[i-2])%mod;	// 模加法同态：等价于完整 f(n) 再取模
	i64 v=f[n];
	cout<<v<<'=';
	if(v==1)	// 1 没有质因子，约定输出 1=1
	{
		cout<<1<<'\n';
		return 0;
	}
	bool first=true;
	for(i64 p=2;p*p<=v;++p)
		if(v%p==0)
		{
			int e=0;
			while(v%p==0)v/=p,++e;
			if(!first)cout<<'*';
			first=false;
			cout<<p;
			if(e>1)cout<<'^'<<e;	// 指数 1 省略 ^1
		}
	if(v>1)	// 剩下的 v 是质数（引理 4）
	{
		if(!first)cout<<'*';
		cout<<v;
	}
	cout<<'\n';
	return 0;
}

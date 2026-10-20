//
// 题目：P1573 栈的操作
// https://www.luogu.com.cn/problem/P1573
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr i64 mod{1000007};   // 1e6+7

i64 qpow(i64 a,i64 b,i64 c=mod)
{
	i64 res=1;
	for(;b;b>>=1,a=a*a%c)
		if(b&1)
			res=res*a%c;
	return res;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	i64 n;
	cin>>n;
	// 设 t=max{t:t(t+1)/2<=n-1},r=(n-1)-t(t+1)/2,答案=(t+r)*2^t+1
	i64 m=n-1;
	i64 t=(i64)((sqrt((double)(8*m+1))-1)/2);
	while((__int128)(t+1)*(t+2)/2<=m)   // 浮点修正:上顶
		++t;
	while(t>0&&(__int128)t*(t+1)/2>m)   // 浮点修正:下压
		--t;
	i64 Tt=t*(t+1)/2;
	i64 r=m-Tt;
	i64 ans=(((t+r)%mod)*qpow(2,t)%mod+1)%mod;
	cout<<ans<<'\n';
	return 0;
}

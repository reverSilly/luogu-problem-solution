//
// 题目：P1516 青蛙的约会
// https://www.luogu.com.cn/problem/P1516
// AC
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

i64 exgcd(i64 a,i64 b,i64&x,i64&y)
{
	if(!b)
	{
		x=1;y=0;
		return a;
	}
	i64 g=exgcd(b,a%b,y,x);
	y-=a/b*x;
	return g;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	i64 x,y,m,n,L;
	if(!(cin>>x>>y>>m>>n>>L))
		return 0;

	// 方程: x + m*t === y + n*t (mod L)  <=>  ((m-n) mod L)*t + L*k = (y-x) mod L
	i64 a=((m-n)%L+L)%L;
	i64 b=((y-x)%L+L)%L;
	i64 t0,k0;
	i64 g=exgcd(a,L,t0,k0);
	if(b%g!=0)
	{
		cout<<"Impossible\n";
		return 0;
	}
	i64 mod=L/g;
	i64 ans=(i64)((__int128)(b/g)*(t0%mod+mod)%mod);
	cout<<ans<<'\n';
	return 0;
}

// 题目：P2817 宋荣子的城堡
// 函数图：条件迫使 p 保持划分 [1..k] / [k+1..n]。
// 外部 (n-k)^(n-k) 任意；内部须唯一含 1 环，拆环成树 ⇒ S(k)=k*k^(k-2)=k^(k-1)。
// 答案 k^(k-1) * (n-k)^(n-k) mod 1e9+7；n=k 时 0^0=1 由 qpow(_,0)=1 处理。
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr i64 p{1'000'000'007};

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

	i64 n,k;
	cin>>n>>k;
	cout<<qpow(k,k-1)*qpow((n-k)%p,n-k)%p<<'\n';
	return 0;
}

//
// 题目：P1647 锁
// https://www.luogu.com.cn/problem/P1647
// AC
#include<bits/stdc++.h>
using namespace std;
using u64=unsigned long long;

int n;
u64 k;

u64 get_s(u64 i)
{
	if(i<=1)
		return i;
	int b=__lg(i);
	u64 p=1ULL<<b;
	u64 ni=(i==p)?(p-1):(i-p-1);
	return p+get_s(ni);
}

u64 solve_max(int b,u64 rem_k)
{
	if(rem_k==(1ULL<<b))
		return (1ULL<<b)-1;
	if(b==1)
		return rem_k==1?0:1;
	u64 half=1ULL<<(b-1);
	if(rem_k<=half)
		return solve_max(b-1,rem_k);
	u64 rem=rem_k-half;
	u64 cand=get_s(half-1);
	if(rem>=2)
		cand=max(cand,solve_max(b-1,rem-1));
	return half+cand;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>k;
	u64 ans=solve_max(n,k);
	for(int i=n-1;i>=0;--i)
		cout<<((ans>>i)&1);
	cout<<'\n';
	return 0;
}

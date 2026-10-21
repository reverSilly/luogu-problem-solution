//
// 题目：P1660 数位平方和
// 暴力基准：对每个 i 独立模拟轨道直至出现重复元素并取全轨道最小值
#include<bits/stdc++.h>
using namespace std;
using i64=long long;

constexpr int mod{10000007};

int k,A,B,pw[10];

int calc_s(int x)
{
	int s=0;
	for(;x;x/=10)
		s+=pw[x%10];
	return s;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>k>>A>>B;
	if(A>B)
		swap(A,B);
	for(int d=0;d<10;++d)
	{
		int r=1;
		for(int i=0;i<k;++i)
			r*=d;
		pw[d]=r;
	}

	i64 ans=0;
	for(int i=A;i<=B;++i)
	{
		unordered_set<int>vis;
		int u=i,mn=i;
		while(!vis.count(u))
		{
			vis.insert(u);
			mn=min(mn,u);
			u=calc_s(u);
		}
		ans=(ans+mn)%mod;
	}
	cout<<ans<<'\n';
	return 0;
}

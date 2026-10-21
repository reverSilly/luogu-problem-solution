//
// 题目：P1561 [USACO12JAN] Mountain Climbing S
// https://www.luogu.com.cn/problem/P1561
// AC
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{25005};         // N<=25000 (+5 松弛)

struct Cow
{
	int u,d;
	// Johnson 法则: 先排 u < d 的组(按 u 升序), 再排 u >= d 的组(按 d 降序)
	friend bool operator<(Cow a,Cow b)
	{
		int ga=(a.u<a.d)?0:1;
		int gb=(b.u<b.d)?0:1;
		if(ga!=gb)
			return ga<gb;
		return ga==0?(a.u<b.u):(a.d>b.d);
	}
}c[mxn];

int n;

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	if(!(cin>>n))
		return 0;
	for(int i=0;i<n;++i)
		cin>>c[i].u>>c[i].d;

	sort(c,c+n);

	long long sum_u=0,finish_d=0;
	for(int i=0;i<n;++i)
	{
		sum_u+=c[i].u;
		finish_d=max(finish_d,sum_u)+c[i].d;
	}
	cout<<finish_d<<'\n';
	return 0;
}

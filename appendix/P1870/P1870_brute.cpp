//
// 题目：P1870 公交车
// https://www.luogu.com.cn/problem/P1870
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};

int n;
long double ac,dist;

// 二分行驶时间，使加速再匀速的路程刚好达到 d。与闭式公式不同
long double travel(long double v)
{
	long double lo=0,hi=1e18;
	for(int it=0;it<120;++it)
	{
		long double mid=(lo+hi)/2;
		long double s;
		long double tacc=v/ac;
		if(mid<=tacc)
			s=0.5L*ac*mid*mid;
		else
			s=0.5L*ac*tacc*tacc+v*(mid-tacc);
		if(s>=dist)
			hi=mid;
		else
			lo=mid;
	}
	return hi;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cout<<fixed<<setprecision(10);

	cin>>n>>ac>>dist;
	long double pre=0;
	vector<long double> out;
	out.reserve(n);
	for(int i=0;i<n;++i)
	{
		long double t,v;
		cin>>t>>v;
		long double cur=t+travel(v);
		if(cur<pre)
			cur=pre;
		pre=cur;
		out.push_back(cur);
	}
	for(long double x:out)
		cout<<(double)x<<'\n';
	return 0;
}

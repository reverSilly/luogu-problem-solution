//
// 题目：P1870 公交车
// https://www.luogu.com.cn/problem/P1870
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{100005};

int n;
double ac,d;
double ans[mxn];

double solo(double v)
{
	double tacc=v/ac;
	double sacc=v*v/(2.0*ac);
	if(sacc>=d)
		return sqrt(2.0*d/ac);
	return tacc+(d-sacc)/v;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cout<<fixed<<setprecision(10);

	cin>>n>>ac>>d;
	double pre=0;
	for(int i=0;i<n;++i)
	{
		double t,v;
		cin>>t>>v;
		double cur=t+solo(v);
		if(cur<pre)
			cur=pre;
		pre=cur;
		ans[i]=cur;
	}
	for(int i=0;i<n;++i)
		cout<<ans[i]<<'\n';
	return 0;
}

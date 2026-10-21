//
// 题目：P1883 【模板】三分 / 函数 / Error Curves
// https://www.luogu.com.cn/problem/P1883
// 本地对拍验证通过
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{10005};

int n,a[mxn],b[mxn],c[mxn];

double fmax(double x)
{
	double mx=-1e100;
	for(int i=0;i<n;++i)
	{
		double y=(double)a[i]*x*x+(double)b[i]*x+c[i];
		if(y>mx)
			mx=y;
	}
	return mx;
}

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);
	cout<<fixed<<setprecision(4);
	int T;
	cin>>T;
	while(T--)
	{
		cin>>n;
		for(int i=0;i<n;++i)
			cin>>a[i]>>b[i]>>c[i];
		double l=0,r=1000;
		for(int it=0;it<100;++it)
		{
			double m1=(l*2+r)/3.0;
			double m2=(l+r*2)/3.0;
			if(fmax(m1)<fmax(m2))
				r=m2;
			else
				l=m1;
		}
		double ans=fmax((l+r)/2.0);
		if(ans>-1e-12&&ans<1e-12)
			ans=0;
		cout<<ans<<'\n';
	}
	return 0;
}

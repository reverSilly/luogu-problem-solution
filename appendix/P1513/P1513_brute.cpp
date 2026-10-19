//
// 题目：P1513 绕钉子的长绳子 (暴力基准)
// https://www.luogu.com.cn/problem/P1513
// 仅用于对拍: 按几何定义逐段累加(公切线段 + 每个钉子处的圆弧), 不套用 2πR 闭式
#include<bits/stdc++.h>
using namespace std;

constexpr int mxn{105};

int n;
double R;
double x[mxn],y[mxn];

int main()
{
	cin.tie(nullptr);
	cout.tie(nullptr);
	ios::sync_with_stdio(false);

	cin>>n>>R;
	for(int i=0;i<n;++i)
		cin>>x[i]>>y[i];

	if(n==1)                 // 只有一根钉子: 绳子就是一圈圆周
	{
		cout<<fixed<<setprecision(2)<<2*acos(-1.0)*R<<'\n';
		return 0;
	}

	double straight=0,arc=0;
	for(int i=0;i<n;++i)
	{
		int p=(i-1+n)%n,q=(i+1)%n;
		double inx=x[i]-x[p],iny=y[i]-y[p];
		double oux=x[q]-x[i],ouy=y[q]-y[i];
		straight+=hypot(oux,ouy);       // 等半径圆的公切线长 = 中心距
		double cr=inx*ouy-iny*oux;      // 逆时针转过的外角
		double dt=inx*oux+iny*ouy;
		double tr=atan2(cr,dt);         // 共线退化时 atan2 可能给出 -π(带符号零), 需归正
		if(tr<0)
			tr+=2*acos(-1.0);
		arc+=R*tr;
	}
	double ans=straight+arc;
	cout<<fixed<<setprecision(2)<<ans<<'\n';
	return 0;
}
